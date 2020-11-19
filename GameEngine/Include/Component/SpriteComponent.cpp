#include "SpriteComponent.h"
#include "../Render/RenderManager.h"
#include "../Resource/Mesh2D.h"
#include "../Resource/Material.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "Transform.h"
#include "../Resource/Animation2DSequence.h"
#include "../Resource/Texture.h"
#include "../Resource/ShaderManager.h"
#include "../GameObject.h"

CSpriteComponent::CSpriteComponent()
    : m_pCurrent(nullptr),m_fScale(1.f)
{
    m_eSceneComponentType = SCENECOMPONENT_TYPE::ST_2D;
    m_eSceneClassType = SCENECOMPONENT_CLASS_TYPE::SPRITE;
}

CSpriteComponent::CSpriteComponent(const CSpriteComponent& com)
    : CPrimitiveComponent(com)
{
    m_mapSpriteInfo.clear();

    //SpriteInfo 복사
    //--------------------------------
    auto iter = com.m_mapSpriteInfo.begin();
    auto iterEnd = com.m_mapSpriteInfo.end();

    for (; iter != iterEnd; ++iter)
    {
        PSpriteInfo pInfo = new SpriteInfo;

        *pInfo = *iter->second;
        pInfo->FinishCallback = nullptr;

        if (pInfo->pAnimation)
            pInfo->pAnimation->AddRef();//Animation2DSequence는 Resource

        m_mapSpriteInfo.insert(make_pair(iter->first, pInfo));

        //Current Sprite Info Setting
        //---------------------------
        if (com.m_pCurrent)
        {
            if (com.m_pCurrent->pAnimation == pInfo->pAnimation)
                m_pCurrent = pInfo;
        }
        //---------------------------

    }
    //--------------------------------
}

CSpriteComponent::~CSpriteComponent()
{
    auto iter = m_mapSpriteInfo.begin();
    auto iterEnd = m_mapSpriteInfo.end();

    for (; iter != iterEnd; ++iter)
    {
        iter->second->pAnimation->DeleteSpriteComponent(this);
        SAFE_RELEASE(iter->second->pAnimation);
        SAFE_DELETE(iter->second);
    }
}

Vector2 CSpriteComponent::GetFrameStart() const
{
    return m_pCurrent->pAnimation->m_vecAnimFrame[m_pCurrent->iFrame].vStart;
}

Vector2 CSpriteComponent::GetFrameEnd() const
{
    return m_pCurrent->pAnimation->m_vecAnimFrame[m_pCurrent->iFrame].vEnd;
}

Vector2 CSpriteComponent::GetTextureSize() const
{
    CTexture* pTexture = m_pCurrent->pAnimation->m_pTexture;

	return Vector2((float)pTexture->GetWidth(),(float)pTexture->GetHeight());
}

void CSpriteComponent::SetTexture(TEXTURE_LINK eLink, CTexture* pTexture, int iShaderType, int iRegister)
{
    m_pMaterial->SetTexture(eLink, pTexture, iShaderType, iRegister);
}

void CSpriteComponent::SetTexture(TEXTURE_LINK eLink, const string& strName, int iShaderType, int iRegister)
{
    m_pMaterial->SetTexture(eLink, strName, iShaderType, iRegister);
}

void CSpriteComponent::SetTexture(TEXTURE_LINK eLink, const string& strName, const TCHAR* pFileName, const string& strPathName, int iShaderType, int iRegister)
{
    m_pMaterial->SetTexture(eLink, strName, pFileName, strPathName,
        iShaderType, iRegister);
}

void CSpriteComponent::SetTexture(TEXTURE_LINK eLink, const string& strName, const TCHAR* pFullPath, int iShaderType, int iRegister)
{
    m_pMaterial->SetTexture(eLink, strName, pFullPath,
        iShaderType, iRegister);
}

void CSpriteComponent::AddSpriteInfo(const string& strName, const string& strAnimName, bool bLoop, float fPlayRate)
{
    PSpriteInfo	pInfo = FindSpriteInfo(strName);

    if (pInfo)
        return;

    pInfo = new SpriteInfo;

    pInfo->pAnimation = m_pScene->GetResourceManager()->FindAnim2DSequence(strAnimName);
    pInfo->bLoop = bLoop;
    pInfo->fPlayRate = fPlayRate;
    pInfo->fPlayTime = 0.f;
    pInfo->iFrame = 0;

    //처음 들어온 정보를 current로 Setting
    //--------------------------------
    if (!m_pCurrent)
        m_pCurrent = pInfo;
    //--------------------------------
    
    pInfo->pAnimation->AddSpriteComponent(this);

    m_mapSpriteInfo.insert(make_pair(strName, pInfo));
}

void CSpriteComponent::AddSpriteInfo(const string& strName, CAnimation2DSequence* pSequence, bool bLoop, float fPlayRate)
{
    PSpriteInfo	pInfo = FindSpriteInfo(strName);

    if (pInfo)
        return;

    pInfo = new SpriteInfo;

    pInfo->pAnimation = pSequence;

    if (pSequence)
        pSequence->AddRef();

    pInfo->bLoop = bLoop;
    pInfo->fPlayRate = fPlayRate;
    pInfo->fPlayTime = 0.f;
    pInfo->iFrame = 0;

    //처음 들어온 정보를 current로 Setting
   //--------------------------------
    if (!m_pCurrent)
        m_pCurrent = pInfo;
    //--------------------------------

    pInfo->pAnimation->AddSpriteComponent(this);

    m_mapSpriteInfo.insert(make_pair(strName, pInfo));
}

void CSpriteComponent::AddSpriteInfo(const string& strName,const string & strSequenceName)
{
    PSpriteInfo pInfo = FindSpriteInfo(strName);

    if (pInfo)
        return;

    pInfo = new SpriteInfo;

    pInfo->pAnimation = m_pScene->GetResourceManager()->FindAnim2DSequence(strSequenceName);
    if (!pInfo->pAnimation)
    {
        delete pInfo;
        return;
    }
    pInfo->fPlayRate = pInfo->pAnimation->m_fPlayRate;
    pInfo->fPlayTime = pInfo->pAnimation->m_fPlayTime;
    pInfo->bLoop = pInfo->pAnimation->m_bLoop;
    pInfo->iFrame = 0;

    if (!m_pCurrent)
        m_pCurrent = pInfo;

    pInfo->pAnimation->AddSpriteComponent(this);

    m_mapSpriteInfo.insert(make_pair(strName, pInfo));
}

void CSpriteComponent::ChangeSprite(const string& strName)
{
    PSpriteInfo pSprite = FindSpriteInfo(strName);

    if (pSprite == m_pCurrent || !pSprite)
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

    SetWorldScale((tCurFrame.vEnd.x - tCurFrame.vStart.x) * m_fScale, (tCurFrame.vEnd.y - tCurFrame.vStart.y) * m_fScale, 1.f);
    //--------------------------

    //Pivot Setting
    if (m_pCurrent->pAnimation->m_vecAnimPivot.size() > m_pCurrent->iFrame)
    {
        const Vector2 & vPivot = m_pCurrent->pAnimation->GetPivot(m_pCurrent->iFrame);

        SetPivot(vPivot.x, vPivot.y, 0.f);
    }

    //자식 Component는 위치 Setting
    //-------------------------------------------------------------------------------------
    if (m_pParent && m_pParent->GetSceneComponentClassType() == SCENECOMPONENT_CLASS_TYPE::SPRITE)
    {
        CSpriteComponent* pParent = static_cast<CSpriteComponent*>(m_pParent);
        PSpriteInfo ParentCurrent = pParent->m_pCurrent;
        if (ParentCurrent->pAnimation->UseRelativePos())
        {
            Vector2 CurPos;
            if (ParentCurrent->pAnimation->GetTargetFramePosInfo(m_pCurrent->pAnimation->GetName(), ParentCurrent->iFrame,m_pCurrent->iFrame, CurPos))
                SetRelativePos(Vector3(CurPos.x * m_fScale, CurPos.y * m_fScale, 0.f));
        }
    }
    //-------------------------------------------------------------------------------------

}

void CSpriteComponent::CallNotify(const string& strName)
{
    m_pObject->CallAnimation2DNotify(strName);
}

PSpriteInfo CSpriteComponent::FindSpriteInfo(const string& strName)
{
    auto	iter = m_mapSpriteInfo.find(strName);

    if (iter == m_mapSpriteInfo.end())
        return nullptr;

    return iter->second;
}

bool CSpriteComponent::Init()
{
    CPrimitiveComponent::Init();

    m_pMesh = m_pScene->GetResourceManager()->GetDefault2DMesh();
    CMaterial* pMaterial = m_pScene->GetResourceManager()->FindMaterial("Sprite");
    SetMaterial(pMaterial);
    SAFE_RELEASE(pMaterial);

    m_pTransform->SetMeshSize(m_pMesh->GetMax() - m_pMesh->GetMin());

    m_pTransform->SetTransformSpace(true);

    return true;
}

void CSpriteComponent::Start()
{
    CPrimitiveComponent::Start();

    auto	iter = m_vecChild.begin();
    auto	iterEnd = m_vecChild.end();

    for (; iter != iterEnd; ++iter)
    {
        (*iter)->Start();
    }
}

void CSpriteComponent::Update(float fTime)
{
    CPrimitiveComponent::Update(fTime);

    if (!m_pCurrent)
        return;

    //현재 실행중인 애니메이션 시간 갱신
    m_pCurrent->fPlayTime += fTime * m_pCurrent->fPlayRate;

    // 애니메이션의 총 실행 시간 / 애니메이션의 프레임 수 = 1프레임당 실행 시간
    float fFrameTime = m_pCurrent->pAnimation->m_fPlayTimeMax / m_pCurrent->pAnimation->m_iFrameMax;

    //자식 Component는 위치 Setting
    //-------------------------------------------------------------------------------------
    if (m_pParent && m_pParent->GetSceneComponentClassType() == SCENECOMPONENT_CLASS_TYPE::SPRITE)
    {
        CSpriteComponent* pParent = static_cast<CSpriteComponent*>(m_pParent);
        PSpriteInfo ParentCurrent = pParent->m_pCurrent;
        if (ParentCurrent->pAnimation->UseRelativePos())
        {
            Vector2 CurPos;
            if (ParentCurrent->pAnimation->GetTargetFramePosInfo(m_pCurrent->pAnimation->GetName(), ParentCurrent->iFrame, m_pCurrent->iFrame, CurPos))
                SetRelativePos(Vector3(CurPos.x * m_fScale, CurPos.y * m_fScale, 0.f));
        }
    }
    //-------------------------------------------------------------------------------------

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

            if (m_pCurrent->FinishCallback)
                m_pCurrent->FinishCallback();

        }
        //-------------------------------------------------------------
        
        Anim2DFrame tCurFrame = m_pCurrent->pAnimation->GetAnimFrame(m_pCurrent->iFrame);

        SetWorldScale((tCurFrame.vEnd.x - tCurFrame.vStart.x) * m_fScale, (tCurFrame.vEnd.y - tCurFrame.vStart.y) * m_fScale, 1.f);

        if (m_pCurrent->pAnimation->m_vecAnimPivot.size() > m_pCurrent->iFrame)
        {
            const Vector2 & vPivot = m_pCurrent->pAnimation->GetPivot(m_pCurrent->iFrame);

            SetPivot(vPivot.x, vPivot.y, 0.f);
        }

    }


    //-----------------------------------------------
}

void CSpriteComponent::PostUpdate(float fTime)
{
    CPrimitiveComponent::PostUpdate(fTime);
}

void CSpriteComponent::Collision(float fTime)
{
    CPrimitiveComponent::Collision(fTime);
}

void CSpriteComponent::PrevRender(float fTime)
{
    CPrimitiveComponent::PrevRender(fTime);
}

void CSpriteComponent::Render(float fTime)
{
    CPrimitiveComponent::Render(fTime);

    //Material정보(Shader,Material상수버퍼,Texture) Setting
    //-------------------------------------
    if (m_pMaterial)
        m_pMaterial->SetMaterial();
    //-------------------------------------

    //현재 애니메이션 정보 Setting
    //-------------------------------
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
    //-------------------------------

    m_pMesh->Render(fTime);
}

void CSpriteComponent::PostRender(float fTime)
{
    CPrimitiveComponent::PostRender(fTime);
}

CSpriteComponent* CSpriteComponent::Clone()
{
    return new CSpriteComponent(*this);
}

void CSpriteComponent::Save(FILE* pFile)
{
    CPrimitiveComponent::Save(pFile);

    fwrite(&m_fScale, sizeof(m_fScale), 1, pFile);
}

void CSpriteComponent::Load(FILE* pFile)
{
    CPrimitiveComponent::Load(pFile);

    fread(&m_fScale, sizeof(m_fScale),1, pFile);

    //기본 Mesh Setting
    //-----------------------------------------------------------------
    //m_pMesh = m_pScene->GetResourceManager()->GetDefault2DMesh();

    //if (m_pMesh)
    //{
    //    CMaterial* pMaterial = m_pMesh->GetMaterial();
    //    CMaterial* pClone = pMaterial->Clone();

    //    SetMaterial(pClone);

    //    //check
    //    pClone->SetShader("SpriteShader");

    //    SAFE_RELEASE(pMaterial);
    //    SAFE_RELEASE(pClone);

    //    m_pTransform->SetMeshSize(m_pMesh->GetMax() - m_pMesh->GetMin());
    //}
    //-----------------------------------------------------------------
}
