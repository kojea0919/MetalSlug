#include "Camera.h"
#include "../Device.h"
#include "../Scene/Scene.h"
#include "../Scene/CameraManager.h"

CCamera::CCamera()
    : m_fViewAngle(90.f),m_fDistance(3000.f)
{
    //Deafult 3D Camera
    m_eSceneComponentType = SCENECOMPONENT_TYPE::ST_2D;
    m_eSceneClassType = SCENECOMPONENT_CLASS_TYPE::CAMERA;
    SetCameraType(CAMERA_TYPE::CAM3D);
}

CCamera::CCamera(const CCamera& com)
    : CSceneComponent(com)
{
}

CCamera::~CCamera()
{
}

void CCamera::SetCameraType(CAMERA_TYPE eType)
{
    m_eCameraType = eType;

    //3D인경우 원근 투영 || 2D,UI인경우 직교 투영
    //------------------------------------------------
    switch (m_eCameraType)
    {
    case CAMERA_TYPE::CAM3D:
    {
        float fRadian = XMConvertToRadians(m_fViewAngle);

        m_matProj = XMMatrixPerspectiveFovLH(fRadian,
            RESOLUTION.iWidth / (float)RESOLUTION.iHeight, 0.3f, m_fDistance);
    }
        break;

    case CAMERA_TYPE::CAM2D:
    case CAMERA_TYPE::CAMUI:
    {
        m_matProj = XMMatrixOrthographicOffCenterLH(0.f,
            (float)RESOLUTION.iWidth, 0.f, (float)RESOLUTION.iHeight,
            0.f, m_fDistance);
    }
        break;
    }

    //------------------------------------------------
}

bool CCamera::Init()
{
    if (!CSceneComponent::Init())
        return false;

    if (m_pScene)
        m_pScene->GetCameraManager()->SetMainCamera(this);

    return true;
}

void CCamera::Start()
{
    CSceneComponent::Start();
}

void CCamera::Update(float fTime)
{
    CSceneComponent::Update(fTime);

    SetRelativePos(GetRelativePos().x, GetRelativePos().y, 0.f);
}

void CCamera::PostUpdate(float fTime)
{
    CSceneComponent::PostUpdate(fTime);

    m_matView.Identity();

    //View행렬 Setting
    //------------------------------------

    //카메라가 UI타입인 경우에는 View행렬은 항등 행렬
    if (m_eCameraType != CAMERA_TYPE::CAMUI)
    {
        //월드 좌표계에서 카메라의 3축 Setting
        //---------------------------------------------
        for (size_t iCnt = 0; iCnt < AXIS_END; ++iCnt)
        {
            memcpy(&m_matView[(int)iCnt][0], &GetWorldAxis((AXIS)iCnt), sizeof(Vector3));
        }
        //---------------------------------------------

        //직교 행렬은 전치행렬 == 역행렬
        m_matView.Transpose();

        Vector3 vPos = GetWorldPos() * -1.f;
        
        //카메라 타입이 2D인경우 Pivot적용
        //---------------------------------------------
        if (m_eCameraType == CAMERA_TYPE::CAM2D)
        {
            Resolution	tRS = RESOLUTION;
            Vector3	vRS = Vector3((float)tRS.iWidth, (float)tRS.iHeight, 0.f);
            vPos = (GetWorldPos() - vRS * GetPivot()) * -1.f;
        }
        //---------------------------------------------

        for (size_t iCnt = 0; iCnt < AXIS_END; ++iCnt)
        {
            m_matView[3][(int)iCnt] = vPos.Dot(GetWorldAxis((AXIS)iCnt));
        }
    }

    //------------------------------------
}

void CCamera::Collision(float fTime)
{
    CSceneComponent::Collision(fTime);
}

void CCamera::PrevRender(float fTime)
{
    CSceneComponent::PrevRender(fTime);
}

void CCamera::Render(float fTime)
{
    CSceneComponent::Render(fTime);
}

void CCamera::PostRender(float fTime)
{
    CSceneComponent::PostRender(fTime);
}

CCamera* CCamera::Clone()
{
    return new CCamera(*this);
}

void CCamera::Save(FILE* pFile)
{
    CSceneComponent::Save(pFile);

    fwrite(&m_eCameraType, sizeof(CAMERA_TYPE), 1, pFile);
    fwrite(&m_fViewAngle, sizeof(float), 1, pFile);
    fwrite(&m_fDistance, sizeof(float), 1, pFile);
}

void CCamera::Load(FILE* pFile)
{
    CSceneComponent::Load(pFile);

    fread(&m_eCameraType, sizeof(CAMERA_TYPE), 1, pFile);
    fread(&m_fViewAngle, sizeof(float), 1, pFile);
    fread(&m_fDistance, sizeof(float), 1, pFile);

    SetCameraType(m_eCameraType);

    //Scene이 존재하면 Scene에 MainCamera로 Setting
    //--------------------------------------------------
    if (m_pScene)
        m_pScene->GetCameraManager()->SetMainCamera(this);
    //--------------------------------------------------
}

