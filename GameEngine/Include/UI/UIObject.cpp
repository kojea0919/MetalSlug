#include "UIObject.h"
#include "UIControl.h"

CUIObject::CUIObject()
	: m_iZOrder(0), m_pViewport(nullptr)
{
	m_pRoot = new CUIControl;
}

CUIObject::CUIObject(const CUIObject& obj)
{
	*this = obj;
}

CUIObject::~CUIObject()
{
	SAFE_RELEASE(m_pRoot);
}

bool CUIObject::Init()
{
	return true;
}

void CUIObject::Start()
{
	m_pRoot->Start();
}

void CUIObject::Update(float fTime)
{
	m_pRoot->Update(fTime);
}

void CUIObject::PostUpdate(float fTime)
{
	m_pRoot->PostUpdate(fTime);
}

void CUIObject::PrevRender()
{
	m_pRoot->PrevRender();
}

void CUIObject::Render()
{
	sort(m_vecControl.begin(), m_vecControl.end(), CUIObject::SortControl);

	size_t	iSize = m_vecControl.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecControl[i]->Render();
	}
}

void CUIObject::PostRender()
{
	m_pRoot->PostRender();
}

CUIObject* CUIObject::Clone()
{
	return new CUIObject(*this);
}

void CUIObject::SetRoot(CUIControl* pRoot)
{
	SAFE_RELEASE(m_pRoot);
	m_pRoot = pRoot;

	if (m_pRoot)
		m_pRoot->AddRef();
}

CUIControl* CUIObject::FindUIControl(const string& strName)
{
	if (!m_pRoot)
		return nullptr;

	return m_pRoot->FindControl(strName);
}

Vector3 CUIObject::GetVelocityScale() const
{
	return m_pRoot->GetVelocityScale();
}

Vector3 CUIObject::GetVelocityRot() const
{
	return m_pRoot->GetVelocityRot();
}

Vector3 CUIObject::GetVelocity() const
{
	return m_pRoot->GetVelocity();
}

float CUIObject::GetCurrentVelocity() const
{
	return m_pRoot->GetCurrentVelocity();
}

void CUIObject::SetInheritScale(bool bInherit)
{
	m_pRoot->SetInheritScale(bInherit);
}

void CUIObject::SetInheritRotX(bool bInherit)
{
	m_pRoot->SetInheritRotX(bInherit);
}

void CUIObject::SetInheritRotY(bool bInherit)
{
	m_pRoot->SetInheritRotY(bInherit);
}

void CUIObject::SetInheritRotZ(bool bInherit)
{
	m_pRoot->SetInheritRotZ(bInherit);
}

void CUIObject::InheritScale()
{
	m_pRoot->InheritScale();
}

void CUIObject::InheritRot()
{
	m_pRoot->InheritRot();
}

void CUIObject::InheritPos()
{
	m_pRoot->InheritPos();
}

void CUIObject::SetRelativeScale(const Vector3& vScale)
{
	m_pRoot->SetRelativeScale(vScale);
}

void CUIObject::SetRelativeScale(float x, float y, float z)
{
	m_pRoot->SetRelativeScale(x, y, z);
}

void CUIObject::SetRelativeRotation(const Vector3& vRot)
{
	m_pRoot->SetRelativeRotation(vRot);
}

void CUIObject::SetRelativeRotation(float x, float y, float z)
{
	m_pRoot->SetRelativeRotation(x, y, z);
}

void CUIObject::SetRelativeRotationX(float x)
{
	m_pRoot->SetRelativeRotationX(x);
}

void CUIObject::SetRelativeRotationY(float y)
{
	m_pRoot->SetRelativeRotationY(y);
}

void CUIObject::SetRelativeRotationZ(float z)
{
	m_pRoot->SetRelativeRotationZ(z);
}

void CUIObject::SetRelativePos(const Vector3& vPos)
{
	m_pRoot->SetRelativePos(vPos);
}

void CUIObject::SetRelativePos(float x, float y, float z)
{
	m_pRoot->SetRelativePos(x, y, z);
}

void CUIObject::AddRelativeScale(const Vector3& vScale)
{
	m_pRoot->AddRelativeScale(vScale);
}

void CUIObject::AddRelativeScale(float x, float y, float z)
{
	m_pRoot->AddRelativeScale(x, y, z);
}

void CUIObject::AddRelativePos(const Vector3& vPos)
{
	m_pRoot->AddRelativePos(vPos);
}

void CUIObject::AddRelativePos(float x, float y, float z)
{
	m_pRoot->AddRelativePos(x, y, z);
}

void CUIObject::AddRelativeRotation(const Vector3& vRot)
{
	m_pRoot->AddRelativeRotation(vRot);
}

void CUIObject::AddRelativeRotation(float x, float y, float z)
{
	m_pRoot->AddRelativeRotation(x, y, z);
}

void CUIObject::AddRelativeRotationX(float x)
{
	m_pRoot->AddRelativeRotationX(x);
}

void CUIObject::AddRelativeRotationY(float y)
{
	m_pRoot->AddRelativeRotationY(y);
}

void CUIObject::AddRelativeRotationZ(float z)
{
	m_pRoot->AddRelativeRotationZ(z);
}

Vector3 CUIObject::GetRelativeScale() const
{
	return m_pRoot->GetRelativeScale();
}

Vector3 CUIObject::GetRelativeRot() const
{
	return m_pRoot->GetRelativeRot();
}

Vector3 CUIObject::GetRelativePos() const
{
	return m_pRoot->GetRelativePos();
}

Vector3 CUIObject::GetRelativeAxis(AXIS eAxis) const
{
	return m_pRoot->GetRelativeAxis(eAxis);
}

void CUIObject::SetWorldScale(const Vector3& vScale)
{
	m_pRoot->SetWorldScale(vScale);
}

void CUIObject::SetWorldScale(float x, float y, float z)
{
	m_pRoot->SetWorldScale(x, y, z);
}

void CUIObject::SetWorldRotation(const Vector3& vRot)
{
	m_pRoot->SetWorldRotation(vRot);
}

void CUIObject::SetWorldRotation(float x, float y, float z)
{
	m_pRoot->SetWorldRotation(x, y, z);
}

void CUIObject::SetWorldRotationX(float x)
{
	m_pRoot->SetWorldRotationX(x);
}

void CUIObject::SetWorldRotationY(float y)
{
	m_pRoot->SetWorldRotationY(y);
}

void CUIObject::SetWorldRotationZ(float z)
{
	m_pRoot->SetWorldRotationZ(z);
}

void CUIObject::SetWorldPos(const Vector3& vPos)
{
	m_pRoot->SetWorldPos(vPos);
}

void CUIObject::SetWorldPos(float x, float y, float z)
{
	m_pRoot->SetWorldPos(x, y, z);
}

void CUIObject::AddWorldScale(const Vector3& vScale)
{
	m_pRoot->AddWorldScale(vScale);
}

void CUIObject::AddWorldScale(float x, float y, float z)
{
	m_pRoot->AddWorldScale(x, y, z);
}

void CUIObject::AddWorldPos(const Vector3& vPos)
{
	m_pRoot->AddWorldPos(vPos);
}

void CUIObject::AddWorldPos(float x, float y, float z)
{
	m_pRoot->AddWorldPos(x, y, z);
}

void CUIObject::AddWorldRotation(const Vector3& vRot)
{
	m_pRoot->AddWorldRotation(vRot);
}

void CUIObject::AddWorldRotation(float x, float y, float z)
{
	m_pRoot->AddWorldRotation(x, y, z);
}

void CUIObject::AddWorldRotationX(float x)
{
	m_pRoot->AddWorldRotationX(x);
}

void CUIObject::AddWorldRotationY(float y)
{
	m_pRoot->AddWorldRotationY(y);
}

void CUIObject::AddWorldRotationZ(float z)
{
	m_pRoot->AddWorldRotationZ(z);
}

void CUIObject::SetPivot(const Vector3& vPivot)
{
	m_pRoot->SetPivot(vPivot);
}

void CUIObject::SetPivot(float x, float y, float z)
{
	m_pRoot->SetPivot(x, y, z);
}

void CUIObject::SetMeshSize(const Vector3& vSize)
{
	m_pRoot->SetMeshSize(vSize);
}

Vector3 CUIObject::GetWorldScale() const
{
	return m_pRoot->GetWorldScale();
}

Vector3 CUIObject::GetWorldRot() const
{
	return m_pRoot->GetWorldRot();
}

Vector3 CUIObject::GetWorldPos() const
{
	return m_pRoot->GetWorldPos();
}

Vector3 CUIObject::GetWorldAxis(AXIS eAxis) const
{
	return m_pRoot->GetWorldAxis(eAxis);
}

Vector3 CUIObject::GetPivot() const
{
	return m_pRoot->GetPivot();
}

Matrix CUIObject::GetScaleMatrix() const
{
	return m_pRoot->GetScaleMatrix();
}

Matrix CUIObject::GetRotMatrix() const
{
	return m_pRoot->GetRotMatrix();
}

Matrix CUIObject::GetTranslationMatrix() const
{
	return m_pRoot->GetTranslationMatrix();
}

Matrix CUIObject::GetWorldMatrix() const
{
	return m_pRoot->GetWorldMatrix();
}

void CUIObject::Save(FILE* pFile)
{
	CRef::Save(pFile);
}

void CUIObject::Load(FILE* pFile)
{
	CRef::Load(pFile);
}

bool CUIObject::SortControl(CUIControl* pSrc, CUIControl* pDest)
{
	return pSrc->GetZOrder() < pDest->GetZOrder();
}
