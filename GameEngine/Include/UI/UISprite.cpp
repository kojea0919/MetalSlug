#include "UISprite.h"
#include "../Resource/Animation2DSequence.h"
#include "../Scene/UIViewport.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/Material.h"
#include "UITransform.h"
#include "../Resource/Mesh2D.h"
#include "../Resource/Texture.h"
#include "../Resource/ShaderManager.h"
#include "../Resource/ResourceManager.h"

CUISprite::CUISprite()
	: m_pCurrent(nullptr)
{
}

CUISprite::CUISprite(const CUISprite& sprite)
	: CUIControl(sprite)
{
}

CUISprite::~CUISprite()
{
	auto iter = m_mapSpriteInfo.begin();
	auto iterEnd = m_mapSpriteInfo.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_RELEASE(iter->second->pAnimation);
		SAFE_DELETE(iter->second);
	}
}

bool CUISprite::Init()
{
	if (!CUIControl::Init())
		return false;

	if (m_pMaterial)
	{
		m_pMaterial->SetShader("SpriteShader");

		m_pTransform->SetMeshSize(m_pBodyMesh->GetMax() - m_pBodyMesh->GetMin());
	}

	return true;
}

void CUISprite::Start()
{
	CUIControl::Start();
}

void CUISprite::Update(float fTime)
{
	CUIControl::Update(fTime);

	if (!m_pCurrent)
		return;

	//현재 실행중인 애니메이션 시간 갱신
	m_pCurrent->fPlayTime += fTime * m_pCurrent->fPlayRate;

	// 애니메이션의 총 실행 시간 / 애니메이션의 프레임 수 = 1프레임당 실행 시간
	float fFrameTime = m_pCurrent->pAnimation->m_fPlayTimeMax / m_pCurrent->pAnimation->m_iFrameMax;

	//시간이 지나서 다음 프레임으로 넘어가는 경우 처리
	//-----------------------------------------------
	if (m_pCurrent->fPlayTime >= fFrameTime)
	{
		m_pCurrent->fPlayTime -= fFrameTime;

		++m_pCurrent->iFrame;

		m_pCurrent->pAnimation->Update(m_pCurrent->iFrame);

		//모든 프레임을 출력한 경우
		//-------------------------------------------------------------
		if (m_pCurrent->iFrame == m_pCurrent->pAnimation->m_iFrameMax)
		{
			if (m_pCurrent->bLoop)
				m_pCurrent->iFrame = 0;

			else
				m_pCurrent->iFrame = m_pCurrent->pAnimation->m_iFrameMax - 1;
		}
		//-------------------------------------------------------------
	}
	//-----------------------------------------------
}

void CUISprite::PostUpdate(float fTime)
{
	CUIControl::PostUpdate(fTime);
}

void CUISprite::PrevRender()
{
	CUIControl::PrevRender();
}

void CUISprite::Render()
{
	CUIControl::Render();

	m_pMaterial->SetMaterial();

	if (m_pCurrent)
	{
		//텍스처가 각각 존재하는 경우 Index Setting
		//-----------------------------------------
		int iIndex = 0;

		if (m_pCurrent->pAnimation->m_pTexture->GetImageType() ==
			IMAGE_TYPE::ARRAY)
			iIndex = m_pCurrent->iFrame;
		//-----------------------------------------

		//해당 인덱스 텍스처의 ShaderResourceView를 Shader에 Setting
		m_pCurrent->pAnimation->m_pTexture->SetShader(0,
			(int)CBUFFER_SHADER_TYPE::CBUFFER_PIXEL,
			iIndex);

		//상수버퍼 Update
		//------------------------------------------------
		m_tCBuffer.iImageType = (int)m_pCurrent->pAnimation->m_pTexture->GetImageType();
		m_tCBuffer.vImageSize.x = (float)m_pCurrent->pAnimation->m_pTexture->GetWidth(iIndex);
		m_tCBuffer.vImageSize.y = (float)m_pCurrent->pAnimation->m_pTexture->GetHeight(iIndex);

		Anim2DFrame tFrame = m_pCurrent->pAnimation->GetAnimFrame(m_pCurrent->iFrame);
		m_tCBuffer.vFrameStart = tFrame.vStart;
		m_tCBuffer.vFrameEnd = tFrame.vEnd;

		GET_SINGLE(CShaderManager)->UpdateCBuffer("Sprite", &m_tCBuffer);
		//------------------------------------------------
	}

	m_pBodyMesh->Render(0.f);
}

void CUISprite::PostRender()
{
	CUIControl::PostRender();
}

CUISprite* CUISprite::Clone()
{
	return new CUISprite(*this);
}

void CUISprite::AddSpriteInfo(const string& strName, const string& strAnimName, bool bLoop, float fPlayRate)
{
	//해당 이름의 SpriteInfo가 존재하면 pass
	//---------------------------------------------
	PUISpriteInfo pInfo = FindSpriteInfo(strName);
	if (pInfo)
		return;
	//---------------------------------------------

	pInfo = new UISpriteInfo;

	//pInfo->pAnimation = m_pViewport->GetScene()->GetResourceManager()->FindAnim2DSequence(strAnimName);
	pInfo->pAnimation = GET_SINGLE(CResourceManager)->FindAnim2DSequence(strAnimName);
	pInfo->bLoop = bLoop;
	pInfo->fPlayRate = fPlayRate;
	pInfo->fPlayTime = 0.f;
	pInfo->iFrame = 0;

	if (!m_pCurrent)
		m_pCurrent = pInfo;

	m_mapSpriteInfo.insert(make_pair(strName, pInfo));
}

void CUISprite::AddSpriteInfo(const string& strName, CAnimation2DSequence* pSequence, bool bLoop, float fPlayRate)
{
	//해당 이름의 SpriteInfo가 존재하면 pass
	//---------------------------------------------
	PUISpriteInfo pInfo = FindSpriteInfo(strName);
	if (pInfo)
		return;
	//---------------------------------------------

	pInfo = new UISpriteInfo;

	pInfo->pAnimation = pSequence;
	pInfo->bLoop = bLoop;
	pInfo->fPlayRate = fPlayRate;
	pInfo->fPlayTime = 0.f;
	pInfo->iFrame = 0;

	if (!m_pCurrent)
		m_pCurrent = pInfo;

	m_mapSpriteInfo.insert(make_pair(strName, pInfo));
}

void CUISprite::AddSpriteInfo(const string& strName, const string& strSequenceName)
{
	//해당 이름의 SpriteInfo가 존재하면 pass
//---------------------------------------------
	PUISpriteInfo pInfo = FindSpriteInfo(strName);
	if (pInfo)
		return;
	//---------------------------------------------

	pInfo = new UISpriteInfo;

	pInfo->pAnimation = m_pViewport->GetScene()->GetResourceManager()->FindAnim2DSequence(strSequenceName);
	if (!pInfo->pAnimation)
	{
		delete pInfo;
		return;
	}

	pInfo->bLoop = pInfo->pAnimation->m_bLoop;
	pInfo->fPlayRate = pInfo->pAnimation->m_fPlayRate;
	pInfo->fPlayTime = pInfo->pAnimation->m_fPlayTime;
	pInfo->iFrame = 0;

	if (!m_pCurrent)
		m_pCurrent = pInfo;

	m_mapSpriteInfo.insert(make_pair(strName, pInfo));
}

void CUISprite::ChangeSprite(const string& strName)
{
	PUISpriteInfo pSprite = FindSpriteInfo(strName);

	if (pSprite == m_pCurrent)
		return;

	m_pCurrent = pSprite;


	//실행 시간과 현재 Frame 초기화
	//--------------------------
	m_pCurrent->fPlayTime = 0.f;
	m_pCurrent->iFrame = 0;
	//--------------------------

	//현재 Frame에 맞는 사각형 크기 Setting
	//--------------------------
	Anim2DFrame tCurFrame = m_pCurrent->pAnimation->GetAnimFrame(m_pCurrent->iFrame);

	SetWorldScale((tCurFrame.vEnd.x - tCurFrame.vStart.x), (tCurFrame.vEnd.y - tCurFrame.vStart.y), 1.f);
	//--------------------------
}

PUISpriteInfo CUISprite::FindSpriteInfo(const string& strName)
{
	return PUISpriteInfo();
}
