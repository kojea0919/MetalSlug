#include "GameObject.h"
#include "Component/SceneComponent.h"
#include "Component/ObjectComponent.h"
#include "Component/StaticMeshComponent.h"
#include "Component/Mesh2DComponent.h"
#include "Component/Camera.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderOBB2D.h"
#include "Component/ColliderRect.h"
#include "Component/ColliderSphere2D.h"
#include "Component/ColliderOBB2D.h"
#include "Component/ColliderPixel.h"

CGameObject::CGameObject()
	: m_bStart(false), m_pScene(nullptr),
	m_iObjType(0)
{
	m_pRootComponent = new CSceneComponent;
	m_pInputObj = new CInputObj;
}

CGameObject::CGameObject(const CGameObject& obj)
{
	*this = obj;

	m_bStart = false;

	m_pInputObj = new CInputObj(*obj.m_pInputObj);
}

CGameObject::~CGameObject()
{
	//에디터에서 삭제하는 함수 호출
	//--------------------------------
	if (m_DestroyEditorFunc)
		m_DestroyEditorFunc(this);
	//--------------------------------

	SAFE_RELEASE(m_pRootComponent);
	SAFE_RELEASE_VECLIST(m_vecObjComponent);
	SAFE_DELETE(m_pInputObj);
}

bool CGameObject::Init()
{
	return true;
}

void CGameObject::Start()
{
	m_bStart = true;

	m_pRootComponent->Start();

	size_t iLen = m_vecObjComponent.size();
	for (size_t iCnt = 0; iCnt < iLen; ++iCnt)
		m_vecObjComponent[iCnt]->Start();
}

void CGameObject::Update(float fTime)
{
	m_pRootComponent->Update(fTime);

	size_t iLen = m_vecObjComponent.size();
	for (size_t iCnt = 0; iCnt < iLen; ++iCnt)
		m_vecObjComponent[iCnt]->Update(fTime);
}

void CGameObject::PostUpdate(float fTime)
{
	m_pRootComponent->PostUpdate(fTime);

	size_t iLen = m_vecObjComponent.size();
	for (size_t iCnt = 0; iCnt < iLen; ++iCnt)
		m_vecObjComponent[iCnt]->PostUpdate(fTime);
}

void CGameObject::Collision(float fTime)
{
	m_pRootComponent->Collision(fTime);

	size_t iLen = m_vecObjComponent.size();
	for (size_t iCnt = 0; iCnt < iLen; ++iCnt)
		m_vecObjComponent[iCnt]->Collision(fTime);
}

void CGameObject::PrevRender(float fTime)
{
	m_pRootComponent->PrevRender(fTime);

	size_t iLen = m_vecObjComponent.size();
	for (size_t iCnt = 0; iCnt < iLen; ++iCnt)
		m_vecObjComponent[iCnt]->PrevRender(fTime);
}

void CGameObject::Render(float fTime)
{
	m_pRootComponent->Render(fTime);

	size_t iLen = m_vecObjComponent.size();
	for (size_t iCnt = 0; iCnt < iLen; ++iCnt)
		m_vecObjComponent[iCnt]->Render(fTime);
}

void CGameObject::PostRender(float fTime)
{
	m_pRootComponent->PostRender(fTime);

	size_t iLen = m_vecObjComponent.size();
	for (size_t iCnt = 0; iCnt < iLen; ++iCnt)
		m_vecObjComponent[iCnt]->PostRender(fTime);
}

void CGameObject::SetRootComponent(CSceneComponent* pRoot)
{
	SAFE_RELEASE(m_pRootComponent);
	m_pRootComponent = pRoot;

	if (m_pRootComponent)
		m_pRootComponent->AddRef();
}

CSceneComponent* CGameObject::GetRootComponent()
{
	if (m_pRootComponent)
		m_pRootComponent->AddRef();

	return m_pRootComponent;
}

CSceneComponent* CGameObject::FindSceneComponent(const string& strName)
{
	if (!m_pRootComponent)
		return nullptr;

	return m_pRootComponent->FindComponent(strName);
}

CObjectComponent* CGameObject::FindObjectComponent(const string& strName)
{
	size_t iLen = m_vecObjComponent.size();
	for (size_t iCnt = 0; iCnt < iLen; ++iCnt)
	{
		if (m_vecObjComponent[iCnt]->GetName() == strName)
		{
			m_vecObjComponent[iCnt]->AddRef();
			return m_vecObjComponent[iCnt];
		}
	}

	return nullptr;
}

void CGameObject::GetAllSceneComponentName(vector<HierarchyName>& vecName)
{
	if (!m_pRootComponent)
		return;

	m_pRootComponent->GetAllComponentName(vecName);
}

void CGameObject::GetAllObjectComponentName(vector<string>& vecName)
{
	size_t iLen = m_vecObjComponent.size();
	for (size_t iCnt = 0; iCnt < iLen; ++iCnt)
		vecName.push_back(m_vecObjComponent[iCnt]->GetName());
}

void CGameObject::CallAnimation2DNotify(const string& strName)
{
}

Vector3 CGameObject::GetVelocityScale() const
{
	return m_pRootComponent->GetVelocityScale();
}

Vector3 CGameObject::GetVelocityRot() const
{
	return m_pRootComponent->GetVelocityRot();
}

Vector3 CGameObject::GetVelocity() const
{
	return m_pRootComponent->GetVelocity();
}

float CGameObject::GetCurrentVelocity() const
{
	return m_pRootComponent->GetCurrentVelocity();
}

void CGameObject::SetInheritScale(bool bInherit)
{
	m_pRootComponent->SetInheritScale(bInherit);
}

void CGameObject::SetInheritRotX(bool bInherit)
{
	m_pRootComponent->SetInheritRotX(bInherit);
}

void CGameObject::SetInheritRotY(bool bInherit)
{
	m_pRootComponent->SetInheritRotY(bInherit);
}

void CGameObject::SetInheritRotZ(bool bInherit)
{
	m_pRootComponent->SetInheritRotZ(bInherit);
}

void CGameObject::InheritScale()
{
	m_pRootComponent->InheritScale();
}

void CGameObject::InheritRot()
{
	m_pRootComponent->InheritRot();
}

void CGameObject::InheritPos()
{
	m_pRootComponent->InheritPos();
}

void CGameObject::SetRelativeScale(const Vector3& vScale)
{
	m_pRootComponent->SetRelativeScale(vScale);
}

void CGameObject::SetRelativeScale(float x, float y, float z)
{
	m_pRootComponent->SetRelativeScale(x, y, z);
}

void CGameObject::SetRelativeRotation(const Vector3& vRot)
{
	m_pRootComponent->SetRelativeRotation(vRot);
}

void CGameObject::SetRelativeRotation(float x, float y, float z)
{
	m_pRootComponent->SetRelativeRotation(x, y, z);
}

void CGameObject::SetRelativeRotationX(float x)
{
	m_pRootComponent->SetRelativeRotationX(x);
}

void CGameObject::SetRelativeRotationY(float y)
{
	m_pRootComponent->SetRelativeRotationY(y);
}

void CGameObject::SetRelativeRotationZ(float z)
{
	m_pRootComponent->SetRelativeRotationZ(z);
}

void CGameObject::SetRelativePos(const Vector3& vPos)
{
	m_pRootComponent->SetRelativePos(vPos);
}

void CGameObject::SetRelativePos(float x, float y, float z)
{
	m_pRootComponent->SetRelativePos(x, y, z);
}

void CGameObject::AddRelativeScale(const Vector3& vScale)
{
	m_pRootComponent->AddRelativeScale(vScale);
}

void CGameObject::AddRelativeScale(float x, float y, float z)
{
	m_pRootComponent->AddRelativeScale(x, y, z);
}

void CGameObject::AddRelativePos(const Vector3& vPos)
{
	m_pRootComponent->AddRelativePos(vPos);
}

void CGameObject::AddRelativePos(float x, float y, float z)
{
	m_pRootComponent->AddRelativePos(x, y, z);
}

void CGameObject::AddRelativeRotation(const Vector3& vRot)
{
	m_pRootComponent->AddRelativeRotation(vRot);
}

void CGameObject::AddRelativeRotation(float x, float y, float z)
{
	m_pRootComponent->AddRelativeRotation(x, y, z);
}

void CGameObject::AddRelativeRotationX(float x)
{
	m_pRootComponent->AddRelativeRotationX(x);
}

void CGameObject::AddRelativeRotationY(float y)
{
	m_pRootComponent->AddRelativeRotationY(y);
}

void CGameObject::AddRelativeRotationZ(float z)
{
	m_pRootComponent->AddRelativeRotationZ(z);
}

Vector3 CGameObject::GetRelativeScale() const
{
	return m_pRootComponent->GetRelativeScale();
}

Vector3 CGameObject::GetRelativeRot() const
{
	return m_pRootComponent->GetRelativeRot();
}

Vector3 CGameObject::GetRelativePos() const
{
	return m_pRootComponent->GetRelativePos();
}

Vector3 CGameObject::GetRelativeAxis(AXIS eAxis) const
{
	return m_pRootComponent->GetRelativeAxis(eAxis);
}

void CGameObject::SetWorldScale(const Vector3& vScale)
{
	m_pRootComponent->SetWorldScale(vScale);
}

void CGameObject::SetWorldScale(float x, float y, float z)
{
	m_pRootComponent->SetWorldScale(x, y, z);
}

void CGameObject::SetWorldRotation(const Vector3& vRot)
{
	m_pRootComponent->SetWorldRotation(vRot);
}

void CGameObject::SetWorldRotation(float x, float y, float z)
{
	m_pRootComponent->SetWorldRotation(x, y, z);
}

void CGameObject::SetWorldRotationX(float x)
{
	m_pRootComponent->SetWorldRotationX(x);
}

void CGameObject::SetWorldRotationY(float y)
{
	m_pRootComponent->SetWorldRotationY(y);
}

void CGameObject::SetWorldRotationZ(float z)
{
	m_pRootComponent->SetWorldRotationZ(z);
}

void CGameObject::SetWorldPos(const Vector3& vPos)
{
	m_pRootComponent->SetWorldPos(vPos);
}

void CGameObject::SetWorldPos(float x, float y, float z)
{
	m_pRootComponent->SetWorldPos(x, y, z);
}

void CGameObject::AddWorldScale(const Vector3& vScale)
{
	m_pRootComponent->AddWorldScale(vScale);
}

void CGameObject::AddWorldScale(float x, float y, float z)
{
	m_pRootComponent->AddWorldScale(x, y, z);
}

void CGameObject::AddWorldPos(const Vector3& vPos)
{
	m_pRootComponent->AddWorldPos(vPos);
}

void CGameObject::AddWorldPos(float x, float y, float z)
{
	m_pRootComponent->AddWorldPos(x, y, z);
}

void CGameObject::AddWorldRotation(const Vector3& vRot)
{
	m_pRootComponent->AddWorldRotation(vRot);
}

void CGameObject::AddWorldRotation(float x, float y, float z)
{
	m_pRootComponent->AddWorldRotation(x, y, z);
}

void CGameObject::AddWorldRotationX(float x)
{
	m_pRootComponent->AddWorldRotationX(x);
}

void CGameObject::AddWorldRotationY(float y)
{
	m_pRootComponent->AddWorldRotationY(y);
}

void CGameObject::AddWorldRotationZ(float z)
{
	m_pRootComponent->AddWorldRotationZ(z);
}

void CGameObject::SetPivot(const Vector3& vPivot)
{
	m_pRootComponent->SetPivot(vPivot);
}

void CGameObject::SetPivot(float x, float y, float z)
{
	m_pRootComponent->SetPivot(x, y, z);
}

void CGameObject::SetMeshSize(const Vector3& vSize)
{
	m_pRootComponent->SetMeshSize(vSize);
}

Vector3 CGameObject::GetWorldScale() const
{
	return m_pRootComponent->GetWorldScale();
}

Vector3 CGameObject::GetWorldRot() const
{
	return m_pRootComponent->GetWorldRot();
}

Vector3 CGameObject::GetWorldPos() const
{
	return m_pRootComponent->GetWorldPos();
}

Vector3 CGameObject::GetWorldAxis(AXIS eAxis) const
{
	return m_pRootComponent->GetWorldAxis(eAxis);
}

Vector3 CGameObject::GetPivot() const
{
	return m_pRootComponent->GetPivot();
}

Matrix CGameObject::GetScaleMatrix() const
{
	return m_pRootComponent->GetScaleMatrix();
}

Matrix CGameObject::GetRotMatrix() const
{
	return m_pRootComponent->GetRotMatrix();
}

Matrix CGameObject::GetTranslationMatrix() const
{
	return m_pRootComponent->GetTranslationMatrix();
}

Matrix CGameObject::GetWorldMatrix() const
{
	return m_pRootComponent->GetWorldMatrix();
}

void CGameObject::Save(FILE* pFile)
{
	CRef::Save(pFile);

	//해당 오브젝트의 모든 SceneComponent를 벡터에 저장
	//-------------------------------------------------
	vector<CSceneComponent*>	vecSceneCom;
	m_pRootComponent->GetAllComponent(vecSceneCom);
	//-------------------------------------------------

	//Component 개수 저장
	//-------------------------------------------------
	int iSize = (int)vecSceneCom.size();
	fwrite(&iSize, sizeof(iSize), 1, pFile);
	//-------------------------------------------------

	//SceneComponent저장
	//-------------------------------------------------
	vector<string> vecParaentName;

	for (int iCnt = 0; iCnt < iSize; ++iCnt)
	{
		SCENECOMPONENT_CLASS_TYPE eType = vecSceneCom[iCnt]->GetSceneComponentClassType();
	
		fwrite(&eType, sizeof(SCENECOMPONENT_CLASS_TYPE), 1, pFile);

		vecParaentName.push_back(vecSceneCom[iCnt]->GetParentName());
		vecSceneCom[iCnt]->Save(pFile);
	}
	//-------------------------------------------------

	//해당 인덱스의 SceneComponent 부모 이름 저장
	//-------------------------------------------------
	for (int iCnt = 0; iCnt < iSize; ++iCnt)
	{
		int iLen = (int)vecParaentName[iCnt].length();
		fwrite(&iLen, sizeof(iLen), 1, pFile);
		fwrite(vecParaentName[iCnt].c_str(), 1, iLen, pFile);
	}
	//-------------------------------------------------

	//ObjectComponent 저장
	//-------------------------------------------------
	iSize = (int)m_vecObjComponent.size();
	fwrite(&iSize, sizeof(iSize), 1, pFile);

	for (int iCnt = 0; iCnt < iSize; ++iCnt)
		m_vecObjComponent[iCnt]->Save(pFile);
	//-------------------------------------------------
}

void CGameObject::Load(FILE* pFile)
{
	CRef::Load(pFile);

	SAFE_RELEASE(m_pRootComponent);

	//SceneComponent Load
	//-------------------------------------------------
	vector<CSceneComponent*>	vecSceneCom;
	
	int iSize = 0;
	fread(&iSize, sizeof(iSize), 1, pFile);

	for (int iCnt = 0; iCnt < iSize; ++iCnt)
	{
		//Class타입 Load
		//------------------------------------
		SCENECOMPONENT_CLASS_TYPE eType;
		fread(&eType, sizeof(eType), 1, pFile);
		//------------------------------------

		//Component Load
		//------------------------------------
		CSceneComponent* pComponent = nullptr;

		switch (eType)
		{
		case SCENECOMPONENT_CLASS_TYPE::SCENE:
			pComponent = new CSceneComponent;
			break;
		case SCENECOMPONENT_CLASS_TYPE::MESH2D:
			pComponent = new CMesh2DComponent;
			break;
		case SCENECOMPONENT_CLASS_TYPE::STATICMESH:
			pComponent = new CStaticMeshComponent;
			break;
		case SCENECOMPONENT_CLASS_TYPE::CAMERA:
			pComponent = new CCamera;
			break;
		case SCENECOMPONENT_CLASS_TYPE::SPRITE:
			pComponent = new CSpriteComponent;
			break;
		case SCENECOMPONENT_CLASS_TYPE::COLLIDERRECT:
			pComponent = new CColliderRect;
			break;
		case SCENECOMPONENT_CLASS_TYPE::COLLIDERSPHERE2D:
			pComponent = new CColliderSphere2D;
			break;
		case SCENECOMPONENT_CLASS_TYPE::COLLIDEROBB2D:
			pComponent = new CColliderOBB2D;
			break;
		case SCENECOMPONENT_CLASS_TYPE::COLLIDERPIXEL:
			pComponent = new CColliderPixel;
		}

		pComponent->m_pScene = m_pScene;
		pComponent->m_pObject = this;
		pComponent->Load(pFile);

		vecSceneCom.push_back(pComponent);
		//------------------------------------
	}
	//-------------------------------------------------



	//SceneComponent 부모, 자식관계 Setting
	//-------------------------------------------------

	for (int iCnt = 0; iCnt < iSize; ++iCnt)
	{
		int iLen = 0;
		char strParentName[256] = {};
		fread(&iLen, sizeof(iLen), 1, pFile);
		fread(strParentName, 1, iLen, pFile);

		if (strcmp(strParentName, "") == 0)
			SetRootComponent(vecSceneCom[iCnt]);

		else
		{
			//Root부터 자신의 부모를 탐색 하여 Setting
			//-------------------------------------------------
			CSceneComponent* pParent = m_pRootComponent->FindComponent(strParentName);

			if (pParent)
			{
				pParent->AddChild(vecSceneCom[iCnt]);

				SAFE_RELEASE(pParent);
			}
			//-------------------------------------------------
		}

		SAFE_RELEASE(vecSceneCom[iCnt]);
	}
	//-------------------------------------------------


	//ObjectComponent Load
	//-------------------------------------------------
	fread(&iSize, sizeof(iSize), 1, pFile);
	for (int iCnt = 0; iCnt < iSize; ++iCnt)
	{
		//m_vecObjComponent[i]->Load(pFile);
	}
	//-------------------------------------------------
}

