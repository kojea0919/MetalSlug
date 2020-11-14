#include "UIControl.h"
#include "UITransform.h"
#include "../Resource/Mesh2D.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Material.h"
#include "../Resource/Texture.h"
#include "../Render/RenderManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneCollision.h"
#include "UIObject.h"

CUIControl::CUIControl()
	: m_pParent(nullptr),m_pViewport(nullptr),m_pScene(nullptr),
	m_bStart(false),m_pBodyMesh(nullptr),m_pUITexture(nullptr),
	m_iZOrder(0)
{
	m_pTransform = new CUITransform;

	m_pTransform->m_pOwner = this;
}

CUIControl::CUIControl(const CUIControl& control)
{
	m_pBodyMesh = control.m_pBodyMesh;

	if (m_pBodyMesh)
		m_pBodyMesh->AddRef();

	m_pUITexture = control.m_pUITexture;

	if (m_pUITexture)
		m_pUITexture->AddRef();

	m_pMaterial = (CMaterial*)control.m_pMaterial->CreateMaterialInstance();

	m_pViewport = nullptr;
	m_bStart = false;
	m_pTransform = control.m_pTransform->Clone();
	m_pScene = control.m_pScene;

	m_vecChild.clear();

	size_t iSize = control.m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		CUIControl* pChild = control.m_vecChild[i]->Clone();

		pChild->m_pParent = this;
		m_vecChild.push_back(pChild);

		pChild->m_pTransform->m_pParent = m_pTransform;

		m_pTransform->m_vecChild.push_back(pChild->m_pTransform);

		pChild->m_pTransform->InheritScale();
		pChild->m_pTransform->InheritRot();
		pChild->m_pTransform->InheritPos();
	}
}

CUIControl::~CUIControl()
{
	SAFE_RELEASE(m_pMaterial);
	SAFE_RELEASE(m_pUITexture);
	SAFE_RELEASE(m_pBodyMesh);
	SAFE_RELEASE_VECLIST(m_vecChild);
	SAFE_DELETE(m_pTransform);
}

int CUIControl::GetZOrder() const
{
	return m_iZOrder + m_pOwner->GetZOrder();
}

void CUIControl::AddChild(CUIControl* pChild,
	const string& strSocketName)
{
	pChild->AddRef();
	pChild->m_pParent = this;
	m_vecChild.push_back(pChild);

	pChild->m_pTransform->m_pParent = m_pTransform;

	m_pTransform->m_vecChild.push_back(pChild->m_pTransform);

	pChild->m_pTransform->InheritScale();
	pChild->m_pTransform->InheritRot();
	pChild->m_pTransform->InheritPos();
}

void CUIControl::DeleteChild(CUIControl* pChild)
{
	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		if (m_vecChild[i] == pChild)
		{
			DeleteChild(m_vecChild[i]);

			auto	iter = m_vecChild.begin() + i;

			m_vecChild.erase(iter);

			auto	iterTr = m_pTransform->m_vecChild.begin() + i;

			m_pTransform->m_vecChild.erase(iterTr);

			break;
		}
	}
}

CUIControl* CUIControl::FindControl(const string& strName)
{
	if (GetName() == strName)
	{
		AddRef();
		return this;
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		CUIControl* pFind = m_vecChild[i]->FindControl(strName);

		if (pFind)
			return pFind;
	}

	return nullptr;
}

void CUIControl::GetAllControlName(vector<HierarchyName>& vecName)
{
	HierarchyName	tName;
	tName.strName = GetName();

	if (m_pParent)
		tName.strParent = m_pParent->GetName();

	vecName.push_back(tName);

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->GetAllControlName(vecName);
	}
}

void CUIControl::GetAllControl(vector<CUIControl*>& vecSceneCom)
{
	vecSceneCom.push_back(this);

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->GetAllControl(vecSceneCom);
	}
}

bool CUIControl::SetTexture(const string& strName)
{
	SAFE_RELEASE(m_pUITexture);
	m_pUITexture = GET_SINGLE(CResourceManager)->FindTexture(strName);

	return true;
}

bool CUIControl::SetTexture(CTexture* pTexture)
{
	SAFE_RELEASE(m_pUITexture);
	m_pUITexture = pTexture;

	if (m_pUITexture)
		m_pUITexture->AddRef();

	return true;
}

void CUIControl::DetatchChild(CUIControl* pChild)
{
	pChild->m_pParent = nullptr;

	pChild->m_pTransform->m_pParent = nullptr;

	pChild->m_pTransform->SetRelativePos(pChild->m_pTransform->GetWorldPos());
	pChild->m_pTransform->SetRelativeRotation(pChild->m_pTransform->GetWorldRot());
	pChild->m_pTransform->SetRelativeScale(pChild->m_pTransform->GetWorldScale());

	pChild->Release();
}

bool CUIControl::Init()
{
	m_pTransform->m_pViewport = m_pViewport;

	//사용하는 Resource Setting
	//--------------------------------------------
	m_pBodyMesh = GET_SINGLE(CResourceManager)->GetDefaultUIMesh();
	m_pUITexture = GET_SINGLE(CResourceManager)->GetDefaultUITexture();

	CMaterial* pMtrl = m_pBodyMesh->GetMaterial();

	m_pMaterial = (CMaterial*)pMtrl->CreateMaterialInstance();

	SAFE_RELEASE(pMtrl);

	//--------------------------------------------

	return true;
}

void CUIControl::Start()
{
	m_bStart = true;

	m_pTransform->Start();

	auto	iter = m_vecChild.begin();
	auto	iterEnd = m_vecChild.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Start();
	}
}

void CUIControl::Update(float fTime)
{
	auto	iter = m_vecChild.begin();
	auto	iterEnd = m_vecChild.end();
	auto	iterTr = m_pTransform->m_vecChild.begin();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			DetatchChild(*iter);
			iter = m_vecChild.erase(iter);
			iterEnd = m_vecChild.end();

			m_pTransform->m_vecChild.erase(iterTr);

			continue;
		}

		if ((*iter)->IsEnable())
		{
			(*iter)->Update(fTime);
		}

		++iter;
		++iterTr;

	}

	m_pTransform->Update(fTime);
}

void CUIControl::PostUpdate(float fTime)
{
	if (m_pScene && m_pOwner->IsEnable())
		m_pScene->GetCollisionManager()->AddUI(this);

	auto	iter = m_vecChild.begin();
	auto	iterEnd = m_vecChild.end();
	auto	iterTr = m_pTransform->m_vecChild.begin();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			DetatchChild(*iter);
			iter = m_vecChild.erase(iter);
			iterEnd = m_vecChild.end();

			m_pTransform->m_vecChild.erase(iterTr);

			continue;
		}

		if ((*iter)->IsEnable())
		{
			(*iter)->PostUpdate(fTime);
		}

		++iter;
		++iterTr;

	}

	m_pTransform->PostUpdate(fTime);

	//Min,Max Setting
	//--------------------------------------
	float fL = GetWorldPos().x - GetWorldPos().x * GetPivot().x;
	float fR = fL + GetWorldScale().x;
	float fB = GetWorldPos().y - GetWorldPos().y * GetPivot().y;
	float fT = fB + GetWorldScale().y;

	m_vMin = Vector3(fL, fB, 0.f);
	m_vMax = Vector3(fR, fT, 1.f);
	//--------------------------------------
}

void CUIControl::PrevRender()
{
	auto	iter = m_vecChild.begin();
	auto	iterEnd = m_vecChild.end();
	auto	iterTr = m_pTransform->m_vecChild.begin();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			DetatchChild(*iter);
			iter = m_vecChild.erase(iter);
			iterEnd = m_vecChild.end();

			m_pTransform->m_vecChild.erase(iterTr);

			continue;
		}

		if ((*iter)->IsEnable())
		{
			(*iter)->PrevRender();
		}

		++iter;
		++iterTr;
	}
}

void CUIControl::Render()
{
	m_pTransform->SetTransform();
}

void CUIControl::PostRender()
{
	auto	iter = m_vecChild.begin();
	auto	iterEnd = m_vecChild.end();
	auto	iterTr = m_pTransform->m_vecChild.begin();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			DetatchChild(*iter);
			iter = m_vecChild.erase(iter);
			iterEnd = m_vecChild.end();

			m_pTransform->m_vecChild.erase(iterTr);

			continue;
		}

		if ((*iter)->IsEnable())
		{
			(*iter)->PostRender();
		}

		++iter;
		++iterTr;
	}
}

CUIControl* CUIControl::Clone()
{
	return new CUIControl(*this);
}

void CUIControl::CollisionMouse(const Vector2& vMousePos, float fTime)
{
}

void CUIControl::CollisionReleaseMouse(const Vector2& vMousePos, float fTime)
{
}

bool CUIControl::UIInMouse(const Vector2& vMousePos)
{
	if (m_vMin.x > vMousePos.x)
		return false;

	else if (m_vMax.x < vMousePos.x)
		return false;

	else if (m_vMin.y > vMousePos.y)
		return false;

	else if (m_vMax.y < vMousePos.y)
		return false;

	return true;
}

Vector3 CUIControl::GetVelocityScale() const
{
	return m_pTransform->GetVelocityScale();
}

Vector3 CUIControl::GetVelocityRot() const
{
	return m_pTransform->GetVelocityRot();
}

Vector3 CUIControl::GetVelocity() const
{
	return m_pTransform->GetVelocity();
}

float CUIControl::GetCurrentVelocity() const
{
	return m_pTransform->GetCurrentVelocity();
}

void CUIControl::SetInheritScale(bool bInherit)
{
	m_pTransform->SetInheritScale(bInherit);
}

void CUIControl::SetInheritRotX(bool bInherit)
{
	m_pTransform->SetInheritRotX(bInherit);
}

void CUIControl::SetInheritRotY(bool bInherit)
{
	m_pTransform->SetInheritRotY(bInherit);
}

void CUIControl::SetInheritRotZ(bool bInherit)
{
	m_pTransform->SetInheritRotZ(bInherit);
}

void CUIControl::InheritScale()
{
	m_pTransform->InheritScale();
}

void CUIControl::InheritRot()
{
	m_pTransform->InheritRot();
}

void CUIControl::InheritPos()
{
	m_pTransform->InheritPos();
}

void CUIControl::SetRelativeScale(const Vector3& vScale)
{
	m_pTransform->SetRelativeScale(vScale);
}

void CUIControl::SetRelativeScale(float x, float y, float z)
{
	m_pTransform->SetRelativeScale(x, y, z);
}

void CUIControl::SetRelativeRotation(const Vector3& vRot)
{
	m_pTransform->SetRelativeRotation(vRot);
}

void CUIControl::SetRelativeRotation(float x, float y, float z)
{
	m_pTransform->SetRelativeRotation(x, y, z);
}

void CUIControl::SetRelativeRotationX(float x)
{
	m_pTransform->SetRelativeRotationX(x);
}

void CUIControl::SetRelativeRotationY(float y)
{
	m_pTransform->SetRelativeRotationY(y);
}

void CUIControl::SetRelativeRotationZ(float z)
{
	m_pTransform->SetRelativeRotationZ(z);
}

void CUIControl::SetRelativePos(const Vector3& vPos)
{
	m_pTransform->SetRelativePos(vPos);
}

void CUIControl::SetRelativePos(float x, float y, float z)
{
	m_pTransform->SetRelativePos(x, y, z);
}

void CUIControl::AddRelativeScale(const Vector3& vScale)
{
	m_pTransform->AddRelativeScale(vScale);
}

void CUIControl::AddRelativeScale(float x, float y, float z)
{
	m_pTransform->AddRelativeScale(x, y, z);
}

void CUIControl::AddRelativePos(const Vector3& vPos)
{
	m_pTransform->AddRelativePos(vPos);
}

void CUIControl::AddRelativePos(float x, float y, float z)
{
	m_pTransform->AddRelativePos(x, y, z);
}

void CUIControl::AddRelativeRotation(const Vector3& vRot)
{
	m_pTransform->AddRelativeRotation(vRot);
}

void CUIControl::AddRelativeRotation(float x, float y, float z)
{
	m_pTransform->AddRelativeRotation(x, y, z);
}

void CUIControl::AddRelativeRotationX(float x)
{
	m_pTransform->AddRelativeRotationX(x);
}

void CUIControl::AddRelativeRotationY(float y)
{
	m_pTransform->AddRelativeRotationY(y);
}

void CUIControl::AddRelativeRotationZ(float z)
{
	m_pTransform->AddRelativeRotationZ(z);
}

Vector3 CUIControl::GetRelativeScale() const
{
	return m_pTransform->GetRelativeScale();
}

Vector3 CUIControl::GetRelativeRot() const
{
	return m_pTransform->GetRelativeRot();
}

Vector3 CUIControl::GetRelativePos() const
{
	return m_pTransform->GetRelativePos();
}

Vector3 CUIControl::GetRelativeAxis(AXIS eAxis) const
{
	return m_pTransform->GetRelativeAxis(eAxis);
}

void CUIControl::SetWorldScale(const Vector3& vScale)
{
	m_pTransform->SetWorldScale(vScale);
}

void CUIControl::SetWorldScale(float x, float y, float z)
{
	m_pTransform->SetWorldScale(x, y, z);
}

void CUIControl::SetWorldRotation(const Vector3& vRot)
{
	m_pTransform->SetWorldRotation(vRot);
}

void CUIControl::SetWorldRotation(float x, float y, float z)
{
	m_pTransform->SetWorldRotation(x, y, z);
}

void CUIControl::SetWorldRotationX(float x)
{
	m_pTransform->SetWorldRotationX(x);
}

void CUIControl::SetWorldRotationY(float y)
{
	m_pTransform->SetWorldRotationY(y);
}

void CUIControl::SetWorldRotationZ(float z)
{
	m_pTransform->SetWorldRotationZ(z);
}

void CUIControl::SetWorldPos(const Vector3& vPos)
{
	m_pTransform->SetWorldPos(vPos);
}

void CUIControl::SetWorldPos(float x, float y, float z)
{
	m_pTransform->SetWorldPos(x, y, z);
}

void CUIControl::AddWorldScale(const Vector3& vScale)
{
	m_pTransform->AddWorldScale(vScale);
}

void CUIControl::AddWorldScale(float x, float y, float z)
{
	m_pTransform->AddWorldScale(x, y, z);
}

void CUIControl::AddWorldPos(const Vector3& vPos)
{
	m_pTransform->AddWorldPos(vPos);
}

void CUIControl::AddWorldPos(float x, float y, float z)
{
	m_pTransform->AddWorldPos(x, y, z);
}

void CUIControl::AddWorldRotation(const Vector3& vRot)
{
	m_pTransform->AddWorldRotation(vRot);
}

void CUIControl::AddWorldRotation(float x, float y, float z)
{
	m_pTransform->AddWorldRotation(x, y, z);
}

void CUIControl::AddWorldRotationX(float x)
{
	m_pTransform->AddWorldRotationX(x);
}

void CUIControl::AddWorldRotationY(float y)
{
	m_pTransform->AddWorldRotationY(y);
}

void CUIControl::AddWorldRotationZ(float z)
{
	m_pTransform->AddWorldRotationZ(z);
}

void CUIControl::SetPivot(const Vector3& vPivot)
{
	m_pTransform->SetPivot(vPivot);
}

void CUIControl::SetPivot(float x, float y, float z)
{
	m_pTransform->SetPivot(x, y, z);
}

void CUIControl::SetMeshSize(const Vector3& vSize)
{
	m_pTransform->SetMeshSize(vSize);
}

Vector3 CUIControl::GetWorldScale() const
{
	return m_pTransform->GetWorldScale();
}

Vector3 CUIControl::GetWorldRot() const
{
	return m_pTransform->GetWorldRot();
}

Vector3 CUIControl::GetWorldPos() const
{
	return m_pTransform->GetWorldPos();
}

Vector3 CUIControl::GetWorldAxis(AXIS eAxis) const
{
	return m_pTransform->GetWorldAxis(eAxis);
}

Vector3 CUIControl::GetPivot() const
{
	return m_pTransform->GetPivot();
}

Matrix CUIControl::GetScaleMatrix() const
{
	return m_pTransform->GetScaleMatrix();
}

Matrix CUIControl::GetRotMatrix() const
{
	return m_pTransform->GetRotMatrix();
}

Matrix CUIControl::GetTranslationMatrix() const
{
	return m_pTransform->GetTranslationMatrix();
}

Matrix CUIControl::GetWorldMatrix() const
{
	return m_pTransform->GetWorldMatrix();
}

void CUIControl::Save(FILE* pFile)
{
	m_pTransform->Save(pFile);
}

void CUIControl::Load(FILE* pFile)
{
	m_pTransform->Load(pFile);
}
