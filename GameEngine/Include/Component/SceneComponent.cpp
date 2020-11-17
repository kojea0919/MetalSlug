#include "SceneComponent.h"
#include "Transform.h"
#include "../Render/RenderManager.h"

CSceneComponent::CSceneComponent()
	: m_eRenderPriority(Render_Priority::RP_OTHER)
{
	m_eComponentType = COMPONENT_TYPE::SCENE;

	m_pTransform = new CTransform;
	m_pTransform->m_pOwner = this;
}

CSceneComponent::CSceneComponent(const CSceneComponent& com)
	: CComponent(com),m_eSceneComponentType(com.m_eSceneComponentType),
	m_eSceneClassType(com.m_eSceneClassType), m_eRenderPriority(com.m_eRenderPriority)
{
	m_pTransform = com.m_pTransform->Clone();

	//자식 Component 복사
	//-------------------------------------
	m_vecChild.clear();

	size_t iLen = com.m_vecChild.size();
	for (size_t iCnt = 0; iCnt < iLen; ++iCnt)
	{
		CSceneComponent* pChild = com.m_vecChild[iCnt]->Clone();

		//자식을 추가하면서 Transform 정보도 Setting
		//--------------------------------
		pChild->m_pParent = this;
		m_vecChild.push_back(pChild);

		CTransform* pChildTransform = pChild->m_pTransform;

		pChildTransform->m_pParent = m_pTransform;

		m_pTransform->m_vecChild.push_back(pChildTransform);

		pChildTransform->InheritScale();
		pChildTransform->InheritRot();
		pChildTransform->InheritPos();

		//--------------------------------
	}
	//-------------------------------------
}

CSceneComponent::~CSceneComponent()
{
	SAFE_RELEASE_VECLIST(m_vecChild);
	SAFE_DELETE(m_pTransform);
}

void CSceneComponent::AddChild(CSceneComponent* pChild, const string& strSocketName)
{
	pChild->AddRef();
	pChild->m_pParent = this;
	m_vecChild.push_back(pChild);

	CTransform* pChildTransform = pChild->m_pTransform;
	pChildTransform->m_pParent = m_pTransform;

	m_pTransform->m_vecChild.push_back(pChildTransform);

	pChildTransform->InheritScale();
	pChildTransform->InheritRot();
	pChildTransform->InheritPos();
}

void CSceneComponent::DeleteChild(CSceneComponent* pChild)
{
	//자식 중 해당 SceneComponent를 찾아서 제거
	//-------------------------------------
	size_t iLen = m_vecChild.size();
	for (size_t iCnt = 0; iCnt < iLen; ++iCnt)
	{
		if (m_vecChild[iCnt] == pChild)
		{
			auto iter = m_vecChild.begin() + iCnt;
			m_vecChild.erase(iter);

			auto Transformiter = m_pTransform->m_vecChild.begin() + iCnt;
			m_pTransform->m_vecChild.erase(Transformiter);

			SAFE_RELEASE(pChild);

			break;
		}
	}
	//-------------------------------------
}

CSceneComponent* CSceneComponent::FindComponent(const string& strName)
{
	if (m_strName == strName)
	{
		AddRef();
		return this;
	}
	
	size_t iLen = m_vecChild.size();
	for (size_t iCnt = 0; iCnt < iLen; ++iCnt)
	{
		CSceneComponent* pFind = m_vecChild[iCnt]->FindComponent(strName);

		if (pFind)
			return pFind;
	}

	return nullptr;
}

void CSceneComponent::GetAllComponentName(vector<HierarchyName>& vecName)
{
	//자신의 이름과 부모의 이름을 HierarchyName 구조체로 Setting하여 vector에 저장
	//------------------------------------------
	HierarchyName tName;
	tName.strName = m_strName;
	
	if (m_pParent)
		tName.strParent = m_pParent->GetName();

	vecName.push_back(tName);

	size_t iLen = m_vecChild.size();
	for (size_t iCnt = 0; iCnt < iLen; ++iCnt)
	{
		m_vecChild[iCnt]->GetAllComponentName(vecName);
	}

	//------------------------------------------
}

void CSceneComponent::GetAllComponent(vector<CSceneComponent*>& vecSceneCom)
{
	vecSceneCom.push_back(this);

	//자식 Component저장
	size_t iLen = m_vecChild.size();
	for (size_t iCnt = 0; iCnt < iLen; ++iCnt)
	{
		m_vecChild[iCnt]->GetAllComponent(vecSceneCom);
	}
	
}

void CSceneComponent::DetatchChild(CSceneComponent* pChild)
{
	//부모 제거
	pChild->m_pParent = nullptr;

	//자식의 Transform 갱신
	//------------------------------------------------
	CTransform* pChildTransform = pChild->m_pTransform;

	pChildTransform->m_pParent = nullptr;
	
	pChildTransform->SetRelativePos(pChildTransform->GetWorldPos());
	pChildTransform->SetRelativeRotation(pChildTransform->GetWorldRot());
	pChildTransform->SetRelativeScale(pChildTransform->GetWorldScale());
	//------------------------------------------------

	SAFE_RELEASE(pChild);
}

bool CSceneComponent::Init()
{
	m_pTransform->m_pScene = m_pScene;

	return true;
}

void CSceneComponent::Start()
{
	CComponent::Start();

	//자식 Component의 Start함수 호출
	//------------------------------
	auto iter = m_vecChild.begin();
	auto iterEnd = m_vecChild.end();

	for (; iter != iterEnd; ++iter)
		(*iter)->Start();
	//------------------------------
}

void CSceneComponent::Update(float fTime)
{
	auto iter = m_vecChild.begin();
	auto iterEnd = m_vecChild.end();
	auto Transformiter = m_pTransform->m_vecChild.begin();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			DetatchChild(*iter);

			iter = m_vecChild.erase(iter);
			iterEnd = m_vecChild.end();

			Transformiter = m_pTransform->m_vecChild.erase(Transformiter);
		}

		if ((*iter)->IsEnable())
		{
			(*iter)->Update(fTime);
		}

		++iter;
		++Transformiter;
	}

	m_pTransform->Update(fTime);
}

void CSceneComponent::PostUpdate(float fTime)
{
	auto iter = m_vecChild.begin();
	auto iterEnd = m_vecChild.end();
	auto Transformiter = m_pTransform->m_vecChild.begin();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			DetatchChild(*iter);

			iter = m_vecChild.erase(iter);
			iterEnd = m_vecChild.end();

			Transformiter = m_pTransform->m_vecChild.erase(Transformiter);
		}

		if ((*iter)->IsEnable())
		{
			(*iter)->PostUpdate(fTime);
		}

		++iter;
		++Transformiter;
	}

	m_pTransform->PostUpdate(fTime);
}

void CSceneComponent::Collision(float fTime)
{
	auto iter = m_vecChild.begin();
	auto iterEnd = m_vecChild.end();
	auto Transformiter = m_pTransform->m_vecChild.begin();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			DetatchChild(*iter);

			iter = m_vecChild.erase(iter);
			iterEnd = m_vecChild.end();

			Transformiter = m_pTransform->m_vecChild.erase(Transformiter);
		}

		if ((*iter)->IsEnable())
		{
			(*iter)->Collision(fTime);
		}

		++iter;
		++Transformiter;
	}
}

void CSceneComponent::PrevRender(float fTime)
{
	auto iter = m_vecChild.begin();
	auto iterEnd = m_vecChild.end();
	auto Transformiter = m_pTransform->m_vecChild.begin();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			DetatchChild(*iter);

			iter = m_vecChild.erase(iter);
			iterEnd = m_vecChild.end();

			Transformiter = m_pTransform->m_vecChild.erase(Transformiter);
		}

		if ((*iter)->IsEnable())
		{
			(*iter)->PrevRender(fTime);
		}

		++iter;
		++Transformiter;
	}
}

void CSceneComponent::Render(float fTime)
{
	m_pTransform->SetTransform();
}

void CSceneComponent::PostRender(float fTime)
{
	auto iter = m_vecChild.begin();
	auto iterEnd = m_vecChild.end();
	auto Transformiter = m_pTransform->m_vecChild.begin();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			DetatchChild(*iter);

			iter = m_vecChild.erase(iter);
			iterEnd = m_vecChild.end();

			Transformiter = m_pTransform->m_vecChild.erase(Transformiter);
		}

		if ((*iter)->IsEnable())
		{
			(*iter)->PostRender(fTime);
		}

		++iter;
		++Transformiter;
	}
}

CSceneComponent* CSceneComponent::Clone()
{
	return new CSceneComponent(*this);
}

void CSceneComponent::InitVelocity()
{
	m_pTransform->InitVelocity();
}

Vector3 CSceneComponent::GetVelocityScale() const
{
	return m_pTransform->GetVelocityScale();
}

Vector3 CSceneComponent::GetVelocityRot() const
{
	return m_pTransform->GetVelocityRot();
}

Vector3 CSceneComponent::GetVelocity() const
{
	return m_pTransform->GetVelocity();
}

float CSceneComponent::GetCurrentVelocity() const
{
	return m_pTransform->GetCurrentVelocity();
}

void CSceneComponent::SetUseParentZValue(bool bUse)
{
	m_pTransform->SetUseParentZValue(bUse);
}

void CSceneComponent::SetInheritScale(bool bInherit)
{
	m_pTransform->SetInheritScale(bInherit);
}

void CSceneComponent::SetInheritRotX(bool bInherit)
{
	m_pTransform->SetInheritRotX(bInherit);
}

void CSceneComponent::SetInheritRotY(bool bInherit)
{
	m_pTransform->SetInheritRotY(bInherit);
}

void CSceneComponent::SetInheritRotZ(bool bInherit)
{
	m_pTransform->SetInheritRotZ(bInherit);
}

void CSceneComponent::InheritScale()
{
	m_pTransform->InheritScale();
}

void CSceneComponent::InheritRot()
{
	m_pTransform->InheritRot();
}

void CSceneComponent::InheritPos()
{
	m_pTransform->InheritPos();
}

void CSceneComponent::SetRelativeScale(const Vector3& vScale)
{
	m_pTransform->SetRelativeScale(vScale);
}

void CSceneComponent::SetRelativeScale(float x, float y, float z)
{
	m_pTransform->SetRelativeScale(x,y,z);
}

void CSceneComponent::SetRelativeRotation(const Vector3& vRot)
{
	m_pTransform->SetRelativeRotation(vRot);
}

void CSceneComponent::SetRelativeRotation(float x, float y, float z)
{
	m_pTransform->SetRelativeRotation(x,y,z);
}

void CSceneComponent::SetRelativeRotationX(float x)
{
	m_pTransform->SetRelativeRotationX(x);
}

void CSceneComponent::SetRelativeRotationY(float y)
{
	m_pTransform->SetRelativeRotationY(y);
}

void CSceneComponent::SetRelativeRotationZ(float z)
{
	m_pTransform->SetRelativeRotationZ(z);
}

void CSceneComponent::SetRelativePos(const Vector3& vPos)
{
	m_pTransform->SetRelativePos(vPos);
}

void CSceneComponent::SetRelativePos(float x, float y, float z)
{
	m_pTransform->SetRelativePos(x, y, z);
}

void CSceneComponent::AddRelativeScale(const Vector3& vScale)
{
	m_pTransform->AddRelativeScale(vScale);
}

void CSceneComponent::AddRelativeScale(float x, float y, float z)
{
	m_pTransform->AddRelativeScale(x, y, z);
}

void CSceneComponent::AddRelativePos(const Vector3& vPos)
{
	m_pTransform->AddRelativePos(vPos);
}

void CSceneComponent::AddRelativePos(float x, float y, float z)
{
	m_pTransform->AddRelativePos(x, y, z);
}

void CSceneComponent::AddRelativeRotation(const Vector3& vRot)
{
	m_pTransform->AddRelativeRotation(vRot);
}

void CSceneComponent::AddRelativeRotation(float x, float y, float z)
{
	m_pTransform->AddRelativeRotation(x, y, z);
}

void CSceneComponent::AddRelativeRotationX(float x)
{
	m_pTransform->AddRelativeRotationX(x);
}

void CSceneComponent::AddRelativeRotationY(float y)
{
	m_pTransform->AddRelativeRotationY(y);
}

void CSceneComponent::AddRelativeRotationZ(float z)
{
	m_pTransform->AddRelativeRotationZ(z);
}

Vector3 CSceneComponent::GetRelativeScale() const
{
	return m_pTransform->GetRelativeScale();
}

Vector3 CSceneComponent::GetRelativeRot() const
{
	return m_pTransform->GetRelativeRot();
}

Vector3 CSceneComponent::GetRelativePos() const
{
	return m_pTransform->GetRelativePos();
}

Vector3 CSceneComponent::GetRelativeAxis(AXIS eAxis) const
{
	return m_pTransform->GetRelativeAxis(eAxis);
}

void CSceneComponent::SetWorldScale(const Vector3& vScale)
{
	m_pTransform->SetWorldScale(vScale);
}

void CSceneComponent::SetWorldScale(float x, float y, float z)
{
	m_pTransform->SetWorldScale(x, y, z);
}

void CSceneComponent::SetWorldRotation(const Vector3& vRot)
{
	m_pTransform->SetWorldRotation(vRot);
}

void CSceneComponent::SetWorldRotation(float x, float y, float z)
{
	m_pTransform->SetWorldRotation(x, y, z);
}

void CSceneComponent::SetWorldRotationX(float x)
{
	m_pTransform->SetWorldRotationX(x);
}

void CSceneComponent::SetWorldRotationY(float y)
{
	m_pTransform->SetWorldRotationY(y);
}

void CSceneComponent::SetWorldRotationZ(float z)
{
	m_pTransform->SetWorldRotationZ(z);
}

void CSceneComponent::SetWorldPos(const Vector3& vPos)
{
	m_pTransform->SetWorldPos(vPos);
}

void CSceneComponent::SetWorldPos(float x, float y, float z)
{
	m_pTransform->SetWorldPos(x, y, z);
}

void CSceneComponent::AddWorldScale(const Vector3& vScale)
{
	m_pTransform->AddWorldScale(vScale);
}

void CSceneComponent::AddWorldScale(float x, float y, float z)
{
	m_pTransform->AddWorldScale(x, y, z);
}

void CSceneComponent::AddWorldPos(const Vector3& vPos)
{
	m_pTransform->AddWorldPos(vPos);
}

void CSceneComponent::AddWorldPos(float x, float y, float z)
{
	m_pTransform->AddWorldPos(x, y, z);
}

void CSceneComponent::AddWorldRotation(const Vector3& vRot)
{
	m_pTransform->AddWorldRotation(vRot);
}

void CSceneComponent::AddWorldRotation(float x, float y, float z)
{
	m_pTransform->AddWorldRotation(x, y, z);
}

void CSceneComponent::AddWorldRotationX(float x)
{
	m_pTransform->AddWorldRotationX(x);
}

void CSceneComponent::AddWorldRotationY(float y)
{
	m_pTransform->AddWorldRotationY(y);
}

void CSceneComponent::AddWorldRotationZ(float z)
{
	m_pTransform->AddWorldRotationZ(z);
}

void CSceneComponent::SetPivot(const Vector3& vPivot)
{
	m_pTransform->SetPivot(vPivot);
}

void CSceneComponent::SetPivot(float x, float y, float z)
{
	m_pTransform->SetPivot(x, y, z);
}

void CSceneComponent::SetMeshSize(const Vector3& vSize)
{
	m_pTransform->SetMeshSize(vSize);
}

Vector3 CSceneComponent::GetWorldScale() const
{
	return m_pTransform->GetWorldScale();
}

Vector3 CSceneComponent::GetWorldRot() const
{
	return m_pTransform->GetWorldRot();
}

Vector3 CSceneComponent::GetWorldPos() const
{
	return m_pTransform->GetWorldPos();
}

Vector3 CSceneComponent::GetWorldAxis(AXIS eAxis) const
{
	return m_pTransform->GetWorldAxis(eAxis);
}

Vector3 CSceneComponent::GetPivot() const
{
	return m_pTransform->GetPivot();
}

Vector3 CSceneComponent::GetMeshSize() const
{
	return m_pTransform->GetMeshSize();
}

Matrix CSceneComponent::GetScaleMatrix() const
{
	return m_pTransform->GetScaleMatrix();
}

Matrix CSceneComponent::GetRotMatrix() const
{
	return m_pTransform->GetRotMatrix();
}

Matrix CSceneComponent::GetTranslationMatrix() const
{
	return m_pTransform->GetTranslationMatrix();
}

Matrix CSceneComponent::GetWorldMatrix() const
{
	return m_pTransform->GetWorldMatrix();
}

void CSceneComponent::Save(FILE* pFile)
{
	CComponent::Save(pFile);

	fwrite(&m_eSceneComponentType, sizeof(m_eSceneComponentType), 1, pFile);
	fwrite(&m_eSceneClassType, sizeof(m_eSceneClassType), 1, pFile);

	m_pTransform->Save(pFile);
}

void CSceneComponent::Load(FILE* pFile)
{
	CComponent::Load(pFile);

	fread(&m_eSceneComponentType, sizeof(m_eSceneComponentType), 1, pFile);
	fread(&m_eSceneClassType, sizeof(m_eSceneClassType), 1, pFile);

	m_pTransform->m_pScene = m_pScene;
	m_pTransform->Load(pFile);
}
