#include "ColliderPixel.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Mesh.h"
#include "../Resource/Material.h"
#include "Collision.h"
#include "ColliderRect.h"
#include "ColliderSphere2D.h"
#include "ColliderOBB2D.h"
#include "../PathManager.h"

CColliderPixel::CColliderPixel()
{
	m_eSceneComponentType = SCENECOMPONENT_TYPE::ST_2D;
	m_eSceneClassType = SCENECOMPONENT_CLASS_TYPE::COLLIDERPIXEL;
	m_eColliderType = COLLIDER_TYPE::PIXEL;

	m_b2D = true;
}

CColliderPixel::CColliderPixel(const CColliderPixel& com)
	: CCollider(com)
{
	m_tInfo = com.m_tInfo;

	m_tInfo.pPixel = new unsigned char[m_tInfo.iWidth * m_tInfo.iHeight * 4];

	memcpy(m_tInfo.pPixel, com.m_tInfo.pPixel,
		m_tInfo.iWidth * m_tInfo.iHeight * 4);
}

CColliderPixel::~CColliderPixel()
{
	SAFE_DELETE_ARRAY(m_tInfo.pPixel);
}

void CColliderPixel::SetPixel(const TCHAR* pFileName, const string& strPathName)
{
	//Full 경로 Setting
	//-----------------------------------------------------------
	TCHAR strFullPath[MAX_PATH] = {};

	const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(strPathName);

	if (pPath)
		lstrcpy(strFullPath, pPath);

	lstrcat(strFullPath, pFileName);
	//-----------------------------------------------------------

	SetPixelFullPath(strFullPath);
}

void CColliderPixel::SetPixelFullPath(const TCHAR* pFullPath)
{
	//이미지 포맷 Setting
	//-----------------------------------------
	TCHAR strExt[_MAX_EXT] = {};

	_wsplitpath_s(pFullPath, nullptr, 0, nullptr, 0, nullptr, 0,
		strExt, _MAX_EXT);

	char _strExt[_MAX_EXT] = {};

	WideCharToMultiByte(CP_ACP, 0, strExt, -1, _strExt,
		lstrlen(strExt), nullptr, nullptr);

	_strupr_s(_strExt);
	//-----------------------------------------

	//이미지 포맷에 따른 Load 함수 호출
	//--------------------------------------------
	ScratchImage Image;

	if (strcmp(_strExt, ".DDS") == 0)
	{
		if (FAILED(LoadFromDDSFile(pFullPath, DDS_FLAGS_NONE, nullptr, Image)))
		{
			return;
		}
	}

	else if (strcmp(_strExt, ".TGA") == 0)
	{
		if (FAILED(LoadFromTGAFile(pFullPath, nullptr, Image)))
		{
			return;
		}
	}

	else
	{
		if (FAILED(LoadFromWICFile(pFullPath, DDS_FLAGS_NONE, nullptr, Image)))
		{
			return;
		}
	}
	//--------------------------------------------

	
	//Info Setting
	//--------------------------------------------
	m_tInfo.iHeight = (int)Image.GetImages()[0].height;
	m_tInfo.iWidth = (int)Image.GetImages()[0].width;

	//32bit가 아닌경우 처리 안함
	//-------------------------------------------------------------------------
	if ((size_t)(m_tInfo.iWidth * m_tInfo.iHeight) != Image.GetPixelsSize() / 4)
		return;
	//-------------------------------------------------------------------------

	uint8_t* a = Image.GetImages()->pixels;

	m_tInfo.pPixel = new unsigned char[Image.GetPixelsSize()];
	memcpy(m_tInfo.pPixel, Image.GetPixels(), Image.GetPixelsSize());

	//버퍼에 상하로 반전하여 관리한다.(출력할때 다시 반전해서 출력)
	//따라서 이를 사용하려면 반전시켜서 사용해야한다.
	//------------------------------------------------------
	if (strcmp(_strExt, ".BMP") == 0)
	{
		size_t iCopySize = (size_t)(m_tInfo.iWidth * 4);

		unsigned char* pLine = new unsigned char[iCopySize + 1];

		int iHalfHeight = m_tInfo.iHeight / 2;
		for (int iCnt = 0; iCnt < iHalfHeight; ++iCnt)
		{
			//pLine에 윗줄부터 복사 시작
			memcpy(pLine, &m_tInfo.pPixel[iCnt * m_tInfo.iWidth * 4], iCopySize);

			//밑에줄을 위에줄과 교체
			memcpy(&m_tInfo.pPixel[iCnt * m_tInfo.iWidth * 4],
				&m_tInfo.pPixel[(m_tInfo.iHeight - 1 - iCnt) * m_tInfo.iWidth * 4],
				iCopySize);

			//pLine을 밑에줄에 복사
			memcpy(&m_tInfo.pPixel[(m_tInfo.iHeight - 1 - iCnt) * m_tInfo.iWidth * 4],
				pLine, iCopySize);
		}
		SAFE_DELETE_ARRAY(pLine);
	}
	//------------------------------------------------------

	//--------------------------------------------

	SetWorldScale((float)m_tInfo.iWidth, (float)m_tInfo.iHeight, 1.f);
}

void CColliderPixel::SetPixelCollisionType(PIXEL_COLLISION_TYPE eType)
{
	m_tInfo.eType = eType;
}

void CColliderPixel::SetIgnoreColor(unsigned char r, unsigned char g, unsigned char b)
{
	m_tInfo.IgnoreColor[0] = r;
	m_tInfo.IgnoreColor[1] = g;
	m_tInfo.IgnoreColor[2] = b;

	SetPixelCollisionType(PIXEL_COLLISION_TYPE::Color_Ignore);
}

void CColliderPixel::SetIgnoreAlpha(unsigned char a)
{
	m_tInfo.IgnoreColor[3] = a;

	SetPixelCollisionType(PIXEL_COLLISION_TYPE::Less_Alpha_Ignore);
}

void CColliderPixel::SetCheckColor(unsigned char r, unsigned char g, unsigned char b)
{
	m_tInfo.CheckColor[0] = r;
	m_tInfo.CheckColor[1] = g;
	m_tInfo.CheckColor[2] = b;

	SetPixelCollisionType(PIXEL_COLLISION_TYPE::Color_Confirm);
}

bool CColliderPixel::Init()
{
	if (!CCollider::Init())
		return false;

	m_pDebugMesh = GET_SINGLE(CResourceManager)->FindMesh("ColliderRect");

	SetInheritRotX(false);
	SetInheritRotY(false);
	SetInheritRotZ(false);

	return true;
}

void CColliderPixel::Start()
{
	CCollider::Start();

	SetWorldScale((float)m_tInfo.iWidth, (float)m_tInfo.iHeight, 1.f);
}

void CColliderPixel::Update(float fTime)
{
	CCollider::Update(fTime);
}

void CColliderPixel::PostUpdate(float fTime)
{
	CCollider::PostUpdate(fTime);

	// 월드공간에서의 위치를 이용해서 사각형 정보를 만든다.
	m_tInfo.tRC.fL = GetWorldPos().x - m_tInfo.iWidth / 2.f;
	m_tInfo.tRC.fR = GetWorldPos().x + m_tInfo.iWidth / 2.f;
	m_tInfo.tRC.fT = GetWorldPos().y + m_tInfo.iHeight / 2.f;
	m_tInfo.tRC.fB = GetWorldPos().y - m_tInfo.iHeight / 2.f;

	m_vMin = Vector3(m_tInfo.tRC.fL, m_tInfo.tRC.fB, 0.f);
	m_vMax = Vector3(m_tInfo.tRC.fR, m_tInfo.tRC.fT, 1.f);
}

void CColliderPixel::Collision(float fTime)
{
	CCollider::Collision(fTime);
}

void CColliderPixel::PrevRender(float fTime)
{
	CCollider::PrevRender(fTime);
}

void CColliderPixel::Render(float fTime)
{
	CCollider::Render(fTime);

	m_pDebugMesh->Render(fTime);
}

void CColliderPixel::PostRender(float fTime)
{
	CCollider::PostRender(fTime);
}

CColliderPixel* CColliderPixel::Clone()
{
	return nullptr;
}

bool CColliderPixel::Collision(CCollider* pCollider)
{
	switch (pCollider->GetColliderType())
	{
	case COLLIDER_TYPE::RECT:
		return CCollision::CollisionRectToPixel((CColliderRect*)pCollider, this);
	case COLLIDER_TYPE::SPHERE2D:
		return CCollision::CollisionSphere2DToPixel((CColliderSphere2D*)pCollider, this);
	case COLLIDER_TYPE::OBB2D:
		return CCollision::CollisionOBB2DToPixel((CColliderOBB2D*)pCollider, this);
	case COLLIDER_TYPE::PIXEL:
		break;
	case COLLIDER_TYPE::LINE:
		break;
	}

	return false;
}

bool CColliderPixel::CollisionMouse(const Vector2& vMouse)
{
	return false;
}

void CColliderPixel::Save(FILE* pFile)
{
	CCollider::Save(pFile);
}

void CColliderPixel::Load(FILE* pFile)
{
	CCollider::Load(pFile);
}
