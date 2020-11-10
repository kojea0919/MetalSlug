
#include "UITransform.h"
#include "../Resource/ShaderManager.h"
#include "../Device.h"
#include "../Scene/CameraManager.h"
#include "../Scene/Scene.h"
#include "UIManager.h"
#include "../Scene/UIViewport.h"

CUITransform::CUITransform() :
	m_pParent(nullptr),
	m_pOwner(nullptr),
	m_pViewport(nullptr),
	m_bInheritScale(false),
	m_bInheritRotX(true),
	m_bInheritRotY(true),
	m_bInheritRotZ(true),
	m_bUpdateScale(true),
	m_bUpdateRot(true),
	m_bUpdatePos(true)
{
	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i];
		m_vWorldAxis[i] = Vector3::Axis[i];
	}

	m_vMeshSize = Vector3(1.f, 1.f, 1.f);
}

CUITransform::CUITransform(const CUITransform& transform)
{
	*this = transform;

	m_vecChild.clear();
	m_pParent = nullptr;
	m_pOwner = nullptr;
	m_pViewport = nullptr;

	m_bUpdateScale = true;
	m_bUpdateRot = true;
	m_bUpdatePos = true;
}

CUITransform::~CUITransform()
{
}

void CUITransform::Start()
{
	m_vVelocityScale = Vector3::Zero;
	m_vVelocityRot = Vector3::Zero;
	m_vVelocity = Vector3::Zero;
}

void CUITransform::Update(float fTime)
{
}

void CUITransform::PostUpdate(float fTime)
{
	if (m_bUpdateScale)
		m_matScale.Scaling(m_vWorldScale);

	if (m_bUpdateRot)
		m_matRot.Rotation(m_vWorldRot);

	m_matTranslation.Translation(m_vWorldPos);

	m_matWorld = m_matScale * m_matRot * m_matTranslation;
}

void CUITransform::SetTransform()
{
	Resolution	m_tRS = RESOLUTION;

	m_tCBuffer.matWorld = m_matWorld;
	m_tCBuffer.matView.Identity();
	m_tCBuffer.matProj = GET_SINGLE(CUIManager)->GetProjMatrix();
	m_tCBuffer.matWV = m_tCBuffer.matWorld * m_tCBuffer.matView;
	m_tCBuffer.matWVP = m_tCBuffer.matWorld * m_tCBuffer.matView * m_tCBuffer.matProj;
	m_tCBuffer.vPivot = m_vPivot;
	m_tCBuffer.vMeshSize = m_vMeshSize;

	// Shader로 행렬을 넘길때는 반드시 Transpose 해주어야 한다.
	// Transpose : 전치행렬
	m_tCBuffer.matWorld.Transpose();
	m_tCBuffer.matView.Transpose();
	m_tCBuffer.matProj.Transpose();
	m_tCBuffer.matWV.Transpose();
	m_tCBuffer.matWVP.Transpose();

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Transform", &m_tCBuffer);

	m_vVelocityScale = Vector3::Zero;
	m_vVelocityRot = Vector3::Zero;
	m_vVelocity = Vector3::Zero;
	m_bUpdateScale = false;
	m_bUpdateRot = false;
	m_bUpdatePos = false;
}

CUITransform* CUITransform::Clone()
{
	return new CUITransform(*this);
}

void CUITransform::Save(FILE* pFile)
{
	fwrite(&m_bInheritScale, sizeof(m_bInheritScale), 1, pFile);
	fwrite(&m_bInheritRotX, sizeof(m_bInheritRotX), 1, pFile);
	fwrite(&m_bInheritRotY, sizeof(m_bInheritRotY), 1, pFile);
	fwrite(&m_bInheritRotZ, sizeof(m_bInheritRotZ), 1, pFile);

	fwrite(&m_vRelativeScale, sizeof(m_vRelativeScale), 1, pFile);
	fwrite(&m_vRelativeRot, sizeof(m_vRelativeRot), 1, pFile);
	fwrite(&m_vRelativePos, sizeof(m_vRelativePos), 1, pFile);
	fwrite(m_vRelativeAxis, sizeof(Vector3), AXIS_END, pFile);

	fwrite(&m_vWorldScale, sizeof(m_vWorldScale), 1, pFile);
	fwrite(&m_vWorldRot, sizeof(m_vWorldRot), 1, pFile);
	fwrite(&m_vWorldPos, sizeof(m_vWorldPos), 1, pFile);
	fwrite(m_vWorldAxis, sizeof(Vector3), AXIS_END, pFile);
	fwrite(&m_vPivot, sizeof(m_vPivot), 1, pFile);
	fwrite(&m_vMeshSize, sizeof(m_vMeshSize), 1, pFile);
}

void CUITransform::Load(FILE* pFile)
{
	m_bUpdateScale = true;
	m_bUpdateRot = true;
	m_bUpdatePos = true;

	fread(&m_bInheritScale, sizeof(m_bInheritScale), 1, pFile);
	fread(&m_bInheritRotX, sizeof(m_bInheritRotX), 1, pFile);
	fread(&m_bInheritRotY, sizeof(m_bInheritRotY), 1, pFile);
	fread(&m_bInheritRotZ, sizeof(m_bInheritRotZ), 1, pFile);

	fread(&m_vRelativeScale, sizeof(m_vRelativeScale), 1, pFile);
	fread(&m_vRelativeRot, sizeof(m_vRelativeRot), 1, pFile);
	fread(&m_vRelativePos, sizeof(m_vRelativePos), 1, pFile);
	fread(m_vRelativeAxis, sizeof(Vector3), AXIS_END, pFile);

	fread(&m_vWorldScale, sizeof(m_vWorldScale), 1, pFile);
	fread(&m_vWorldRot, sizeof(m_vWorldRot), 1, pFile);
	fread(&m_vWorldPos, sizeof(m_vWorldPos), 1, pFile);
	fread(m_vWorldAxis, sizeof(Vector3), AXIS_END, pFile);
	fread(&m_vPivot, sizeof(m_vPivot), 1, pFile);
	fread(&m_vMeshSize, sizeof(m_vMeshSize), 1, pFile);
}

void CUITransform::InheritScale()
{
	if (m_bInheritScale)
		m_vWorldScale = m_vRelativeScale * m_pParent->GetWorldScale();

	m_bUpdateScale = true;

	size_t	iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritScale();
	}
}

void CUITransform::InheritRot()
{
	if (m_bInheritRotX)
		m_vWorldRot.x = m_vRelativeRot.x + m_pParent->GetWorldRot().x;

	if (m_bInheritRotY)
		m_vWorldRot.y = m_vRelativeRot.y + m_pParent->GetWorldRot().y;

	if (m_bInheritRotZ)
		m_vWorldRot.z = m_vRelativeRot.z + m_pParent->GetWorldRot().z;

	if (m_pParent)
		InheritPos();

	Matrix	matRot;
	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_vWorldAxis[i].Normalize();
	}

	m_bUpdateRot = true;

	size_t	iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CUITransform::InheritPos()
{
	Matrix	matRot;

	// 부모의 회전정보를 이용해서 회전행렬을 만들어준다.
	matRot.Rotation(m_pParent->GetWorldRot());

	// 회전의 중심점이 원점기준으로 되어 있으므로 부모의 위치를 기준으로
	// 회전할 수 있게 회전의 중심점을 변경한다.
	memcpy(&matRot._41, &m_pParent->GetWorldPos(), sizeof(Vector3));

	m_vWorldPos = m_vRelativePos.TransformCoord(matRot);

	m_bUpdatePos = true;

	size_t	iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritPos();
	}
}

void CUITransform::SetRelativeScale(const Vector3& vScale)
{
	m_vVelocityScale += vScale - m_vRelativeScale;

	m_vRelativeScale = vScale;

	m_vWorldScale = m_vRelativeScale;

	if (m_pParent && m_bInheritScale)
		m_vWorldScale = m_vRelativeScale * m_pParent->GetWorldScale();

	//m_matScale.Scaling(m_vWorldScale);
	m_bUpdateScale = true;

	// 자식이 있을경우 모든 자식을 반복하며 갱신된 Scale정보를 적용하게 한다.
	size_t	iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritScale();
	}
}

void CUITransform::SetRelativeScale(float x, float y, float z)
{
	m_vVelocityScale += Vector3(x, y, z) - m_vRelativeScale;

	m_vRelativeScale = Vector3(x, y, z);

	m_vWorldScale = m_vRelativeScale;

	if (m_pParent && m_bInheritScale)
		m_vWorldScale = m_vRelativeScale * m_pParent->GetWorldScale();

	//m_matScale.Scaling(m_vWorldScale);
	m_bUpdateScale = true;

	// 자식이 있을경우 모든 자식을 반복하며 갱신된 Scale정보를 적용하게 한다.
	size_t	iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritScale();
	}
}

void CUITransform::SetRelativeRotation(const Vector3& vRot)
{
	m_vVelocityRot += vRot - m_vRelativeRot;

	m_vRelativeRot = vRot;

	m_vWorldRot = m_vRelativeRot;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vWorldRot.x = m_vRelativeRot.x + m_pParent->GetWorldRot().x;

		if (m_bInheritRotY)
			m_vWorldRot.y = m_vRelativeRot.y + m_pParent->GetWorldRot().y;

		if (m_bInheritRotZ)
			m_vWorldRot.z = m_vRelativeRot.z + m_pParent->GetWorldRot().z;
	}

	Matrix	matRot;
	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_vWorldAxis[i].Normalize();
	}

	//m_matRot.Rotation(m_vWorldScale);
	m_bUpdateRot = true;

	// 자식이 있을경우 모든 자식을 반복하며 갱신된 Scale정보를 적용하게 한다.
	size_t	iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CUITransform::SetRelativeRotation(float x, float y, float z)
{
	m_vVelocityRot += Vector3(x, y, z) - m_vRelativeRot;

	m_vRelativeRot = Vector3(x, y, z);

	m_vWorldRot = m_vRelativeRot;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vWorldRot.x = m_vRelativeRot.x + m_pParent->GetWorldRot().x;

		if (m_bInheritRotY)
			m_vWorldRot.y = m_vRelativeRot.y + m_pParent->GetWorldRot().y;

		if (m_bInheritRotZ)
			m_vWorldRot.z = m_vRelativeRot.z + m_pParent->GetWorldRot().z;
	}

	Matrix	matRot;
	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_vWorldAxis[i].Normalize();
	}

	//m_matRot.Rotation(m_vWorldScale);
	m_bUpdateRot = true;

	// 자식이 있을경우 모든 자식을 반복하며 갱신된 Scale정보를 적용하게 한다.
	size_t	iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CUITransform::SetRelativeRotationX(float x)
{
	m_vVelocityRot.x += x - m_vRelativeRot.x;

	m_vRelativeRot.x = x;

	m_vWorldRot = m_vRelativeRot;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vWorldRot.x = m_vRelativeRot.x + m_pParent->GetWorldRot().x;
	}

	Matrix	matRot;
	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_vWorldAxis[i].Normalize();
	}

	//m_matRot.Rotation(m_vWorldScale);
	m_bUpdateRot = true;

	// 자식이 있을경우 모든 자식을 반복하며 갱신된 Scale정보를 적용하게 한다.
	size_t	iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CUITransform::SetRelativeRotationY(float y)
{
	m_vVelocityRot.y += y - m_vRelativeRot.y;

	m_vRelativeRot.y = y;

	m_vWorldRot = m_vRelativeRot;

	if (m_pParent)
	{
		if (m_bInheritRotY)
			m_vWorldRot.y = m_vRelativeRot.y + m_pParent->GetWorldRot().y;
	}

	Matrix	matRot;
	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_vWorldAxis[i].Normalize();
	}

	//m_matRot.Rotation(m_vWorldScale);
	m_bUpdateRot = true;

	// 자식이 있을경우 모든 자식을 반복하며 갱신된 Scale정보를 적용하게 한다.
	size_t	iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CUITransform::SetRelativeRotationZ(float z)
{
	m_vVelocityRot.z += z - m_vRelativeRot.z;

	m_vRelativeRot.z = z;

	m_vWorldRot = m_vRelativeRot;

	if (m_pParent)
	{
		if (m_bInheritRotZ)
			m_vWorldRot.z = m_vRelativeRot.z + m_pParent->GetWorldRot().z;
	}

	Matrix	matRot;
	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_vWorldAxis[i].Normalize();
	}

	//m_matRot.Rotation(m_vWorldScale);
	m_bUpdateRot = true;

	// 자식이 있을경우 모든 자식을 반복하며 갱신된 Scale정보를 적용하게 한다.
	size_t	iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CUITransform::SetRelativePos(const Vector3& vPos)
{
	m_vVelocity += (vPos - m_vRelativePos);

	m_vRelativePos = vPos;

	m_vWorldPos = m_vRelativePos;

	if (m_pParent)
	{
		Matrix	matRot;

		// 부모의 회전정보를 이용해서 회전행렬을 만들어준다.
		matRot.Rotation(m_pParent->GetWorldRot());

		// 회전의 중심점이 원점기준으로 되어 있으므로 부모의 위치를 기준으로
		// 회전할 수 있게 회전의 중심점을 변경한다.
		memcpy(&matRot._41, &m_pParent->GetWorldPos(), sizeof(Vector3));

		m_vWorldPos = m_vRelativePos.TransformCoord(matRot);
	}

	//m_matScale.Scaling(m_vWorldScale);
	m_bUpdatePos = true;

	// 자식이 있을경우 모든 자식을 반복하며 갱신된 Scale정보를 적용하게 한다.
	size_t	iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritPos();
	}
}

void CUITransform::SetRelativePos(float x, float y, float z)
{
	m_vVelocity += Vector3(x, y, z) - m_vRelativePos;

	m_vRelativePos = Vector3(x, y, z);

	m_vWorldPos = m_vRelativePos;

	if (m_pParent)
	{
		Matrix	matRot;

		// 부모의 회전정보를 이용해서 회전행렬을 만들어준다.
		matRot.Rotation(m_pParent->GetWorldRot());

		// 회전의 중심점이 원점기준으로 되어 있으므로 부모의 위치를 기준으로
		// 회전할 수 있게 회전의 중심점을 변경한다.
		memcpy(&matRot._41, &m_pParent->GetWorldPos(), sizeof(Vector3));

		m_vWorldPos = m_vRelativePos.TransformCoord(matRot);
	}

	//m_matScale.Scaling(m_vWorldScale);
	m_bUpdatePos = true;

	// 자식이 있을경우 모든 자식을 반복하며 갱신된 Scale정보를 적용하게 한다.
	size_t	iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritPos();
	}
}

void CUITransform::AddRelativeScale(const Vector3& vScale)
{
	m_vVelocityScale += vScale;

	m_vRelativeScale += vScale;

	m_vWorldScale = m_vRelativeScale;

	if (m_pParent && m_bInheritScale)
		m_vWorldScale = m_vRelativeScale * m_pParent->GetWorldScale();

	m_bUpdateScale = true;

	// 자식이 있을경우 모든 자식을 반복하며 갱신된 Scale정보를 적용하게 한다.
	size_t	iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritScale();
	}
}

void CUITransform::AddRelativeScale(float x, float y, float z)
{
	m_vVelocityScale += Vector3(x, y, z);

	m_vRelativeScale += Vector3(x, y, z);

	m_vWorldScale = m_vRelativeScale;

	if (m_pParent && m_bInheritScale)
		m_vWorldScale = m_vRelativeScale * m_pParent->GetWorldScale();

	m_bUpdateScale = true;

	// 자식이 있을경우 모든 자식을 반복하며 갱신된 Scale정보를 적용하게 한다.
	size_t	iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritScale();
	}
}

void CUITransform::AddRelativePos(const Vector3& vPos)
{
	m_vVelocity += vPos;

	m_vRelativePos += vPos;

	m_vWorldPos = m_vRelativePos;

	if (m_pParent)
	{
		Matrix	matRot;

		// 부모의 회전정보를 이용해서 회전행렬을 만들어준다.
		matRot.Rotation(m_pParent->GetWorldRot());

		// 회전의 중심점이 원점기준으로 되어 있으므로 부모의 위치를 기준으로
		// 회전할 수 있게 회전의 중심점을 변경한다.
		memcpy(&matRot._41, &m_pParent->GetWorldPos(), sizeof(Vector3));

		m_vWorldPos = m_vRelativePos.TransformCoord(matRot);
	}

	//m_matScale.Scaling(m_vWorldScale);
	m_bUpdatePos = true;

	// 자식이 있을경우 모든 자식을 반복하며 갱신된 Scale정보를 적용하게 한다.
	size_t	iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritPos();
	}
}

void CUITransform::AddRelativePos(float x, float y, float z)
{
	m_vVelocity += Vector3(x, y, z);

	m_vRelativePos += Vector3(x, y, z);

	m_vWorldPos = m_vRelativePos;

	if (m_pParent)
	{
		Matrix	matRot;

		// 부모의 회전정보를 이용해서 회전행렬을 만들어준다.
		matRot.Rotation(m_pParent->GetWorldRot());

		// 회전의 중심점이 원점기준으로 되어 있으므로 부모의 위치를 기준으로
		// 회전할 수 있게 회전의 중심점을 변경한다.
		memcpy(&matRot._41, &m_pParent->GetWorldPos(), sizeof(Vector3));

		m_vWorldPos = m_vRelativePos.TransformCoord(matRot);
	}

	//m_matScale.Scaling(m_vWorldScale);
	m_bUpdatePos = true;

	// 자식이 있을경우 모든 자식을 반복하며 갱신된 Scale정보를 적용하게 한다.
	size_t	iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritPos();
	}
}

void CUITransform::AddRelativeRotation(const Vector3& vRot)
{
	m_vVelocityRot += vRot;

	m_vRelativeRot += vRot;

	m_vWorldRot = m_vRelativeRot;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vWorldRot.x = m_vRelativeRot.x + m_pParent->GetWorldRot().x;

		if (m_bInheritRotY)
			m_vWorldRot.y = m_vRelativeRot.y + m_pParent->GetWorldRot().y;

		if (m_bInheritRotZ)
			m_vWorldRot.z = m_vRelativeRot.z + m_pParent->GetWorldRot().z;
	}

	Matrix	matRot;
	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_vWorldAxis[i].Normalize();
	}

	m_bUpdateRot = true;

	// 자식이 있을경우 모든 자식을 반복하며 갱신된 Scale정보를 적용하게 한다.
	size_t	iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CUITransform::AddRelativeRotation(float x, float y, float z)
{
	m_vVelocityRot += Vector3(x, y, z);

	m_vRelativeRot += Vector3(x, y, z);

	m_vWorldRot = m_vRelativeRot;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vWorldRot.x = m_vRelativeRot.x + m_pParent->GetWorldRot().x;

		if (m_bInheritRotY)
			m_vWorldRot.y = m_vRelativeRot.y + m_pParent->GetWorldRot().y;

		if (m_bInheritRotZ)
			m_vWorldRot.z = m_vRelativeRot.z + m_pParent->GetWorldRot().z;
	}

	Matrix	matRot;
	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_vWorldAxis[i].Normalize();
	}

	m_bUpdateRot = true;

	// 자식이 있을경우 모든 자식을 반복하며 갱신된 Scale정보를 적용하게 한다.
	size_t	iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CUITransform::AddRelativeRotationX(float x)
{
	m_vVelocityRot.x += x;

	m_vRelativeRot.x += x;

	m_vWorldRot.x = m_vRelativeRot.x;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vWorldRot.x = m_vRelativeRot.x + m_pParent->GetWorldRot().x;
	}

	Matrix	matRot;
	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_vWorldAxis[i].Normalize();
	}

	m_bUpdateRot = true;

	// 자식이 있을경우 모든 자식을 반복하며 갱신된 Scale정보를 적용하게 한다.
	size_t	iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CUITransform::AddRelativeRotationY(float y)
{
	m_vVelocityRot.y += y;

	m_vRelativeRot.y += y;

	m_vWorldRot.y = m_vRelativeRot.y;

	if (m_pParent)
	{
		if (m_bInheritRotY)
			m_vWorldRot.y = m_vRelativeRot.y + m_pParent->GetWorldRot().y;
	}

	Matrix	matRot;
	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_vWorldAxis[i].Normalize();
	}

	m_bUpdateRot = true;

	// 자식이 있을경우 모든 자식을 반복하며 갱신된 Scale정보를 적용하게 한다.
	size_t	iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CUITransform::AddRelativeRotationZ(float z)
{
	m_vVelocityRot.z += z;

	m_vRelativeRot.z += z;

	m_vWorldRot.z = m_vRelativeRot.z;

	if (m_pParent)
	{
		if (m_bInheritRotZ)
			m_vWorldRot.z = m_vRelativeRot.z + m_pParent->GetWorldRot().z;
	}

	Matrix	matRot;
	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_vWorldAxis[i].Normalize();
	}

	m_bUpdateRot = true;

	// 자식이 있을경우 모든 자식을 반복하며 갱신된 Scale정보를 적용하게 한다.
	size_t	iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

Vector3 CUITransform::GetRelativeScale() const
{
	return m_vRelativeScale;
}

Vector3 CUITransform::GetRelativeRot() const
{
	return m_vRelativeRot;
}

Vector3 CUITransform::GetRelativePos() const
{
	return m_vRelativePos;
}

Vector3 CUITransform::GetRelativeAxis(AXIS eAxis) const
{
	return m_vRelativeAxis[eAxis];
}

Vector3 CUITransform::GetWorldScale() const
{
	return m_vWorldScale;
}

Vector3 CUITransform::GetWorldRot() const
{
	return m_vWorldRot;
}

Vector3 CUITransform::GetWorldPos() const
{
	return m_vWorldPos;
}

Vector3 CUITransform::GetWorldAxis(AXIS eAxis) const
{
	return m_vWorldAxis[eAxis];
}

Vector3 CUITransform::GetPivot() const
{
	return m_vPivot;
}

Matrix CUITransform::GetScaleMatrix() const
{
	return m_matScale;
}

Matrix CUITransform::GetRotMatrix() const
{
	return m_matRot;
}

Matrix CUITransform::GetTranslationMatrix() const
{
	return m_matTranslation;
}

Matrix CUITransform::GetWorldMatrix() const
{
	return m_matWorld;
}

void CUITransform::SetWorldScale(const Vector3& vScale)
{
	m_vVelocityScale += vScale - m_vWorldScale;

	m_vWorldScale = vScale;

	m_vRelativeScale = m_vWorldScale;

	if (m_pParent && m_bInheritScale)
		m_vRelativeScale = m_vWorldScale / m_pParent->GetWorldScale();

	//m_matScale.Scaling(m_vWorldScale);
	m_bUpdateScale = true;

	// 자식이 있을경우 모든 자식을 반복하며 갱신된 Scale정보를 적용하게 한다.
	size_t	iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritScale();
	}
}

void CUITransform::SetWorldScale(float x, float y, float z)
{
	m_vVelocityScale += Vector3(x, y, z) - m_vWorldScale;

	m_vWorldScale = Vector3(x, y, z);

	m_vRelativeScale = m_vWorldScale;

	if (m_pParent && m_bInheritScale)
		m_vRelativeScale = m_vWorldScale / m_pParent->GetWorldScale();

	//m_matScale.Scaling(m_vWorldScale);
	m_bUpdateScale = true;

	// 자식이 있을경우 모든 자식을 반복하며 갱신된 Scale정보를 적용하게 한다.
	size_t	iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritScale();
	}
}

void CUITransform::SetWorldRotation(const Vector3& vRot)
{
	m_vVelocityRot += vRot - m_vWorldRot;

	m_vWorldRot = vRot;

	m_vRelativeRot = m_vWorldRot;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vRelativeRot.x = m_vWorldRot.x - m_pParent->GetWorldRot().x;

		if (m_bInheritRotY)
			m_vRelativeRot.y = m_vWorldRot.y - m_pParent->GetWorldRot().y;

		if (m_bInheritRotZ)
			m_vRelativeRot.z = m_vWorldRot.z - m_pParent->GetWorldRot().z;
	}

	Matrix	matRot;
	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_vWorldAxis[i].Normalize();
	}

	//m_matScale.Scaling(m_vWorldScale);
	m_bUpdateRot = true;

	// 자식이 있을경우 모든 자식을 반복하며 갱신된 Scale정보를 적용하게 한다.
	size_t	iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CUITransform::SetWorldRotation(float x, float y, float z)
{
	m_vVelocityRot += Vector3(x, y, z) - m_vWorldRot;

	m_vWorldRot = Vector3(x, y, z);

	m_vRelativeRot = m_vWorldRot;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vRelativeRot.x = m_vWorldRot.x - m_pParent->GetWorldRot().x;

		if (m_bInheritRotY)
			m_vRelativeRot.y = m_vWorldRot.y - m_pParent->GetWorldRot().y;

		if (m_bInheritRotZ)
			m_vRelativeRot.z = m_vWorldRot.z - m_pParent->GetWorldRot().z;
	}

	Matrix	matRot;
	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_vWorldAxis[i].Normalize();
	}

	//m_matScale.Scaling(m_vWorldScale);
	m_bUpdateRot = true;

	// 자식이 있을경우 모든 자식을 반복하며 갱신된 Scale정보를 적용하게 한다.
	size_t	iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CUITransform::SetWorldRotationX(float x)
{
	m_vVelocityRot.x += x - m_vWorldRot.x;

	m_vWorldRot.x = x;

	m_vRelativeRot.x = m_vWorldRot.x;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vRelativeRot.x = m_vWorldRot.x - m_pParent->GetWorldRot().x;
	}

	Matrix	matRot;
	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_vWorldAxis[i].Normalize();
	}

	//m_matScale.Scaling(m_vWorldScale);
	m_bUpdateRot = true;

	// 자식이 있을경우 모든 자식을 반복하며 갱신된 Scale정보를 적용하게 한다.
	size_t	iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CUITransform::SetWorldRotationY(float y)
{
	m_vVelocityRot.y += y - m_vWorldRot.y;

	m_vWorldRot.y = y;

	m_vRelativeRot.y = m_vWorldRot.y;

	if (m_pParent)
	{
		if (m_bInheritRotY)
			m_vRelativeRot.y = m_vWorldRot.y - m_pParent->GetWorldRot().y;
	}

	Matrix	matRot;
	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_vWorldAxis[i].Normalize();
	}

	//m_matScale.Scaling(m_vWorldScale);
	m_bUpdateRot = true;

	// 자식이 있을경우 모든 자식을 반복하며 갱신된 Scale정보를 적용하게 한다.
	size_t	iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CUITransform::SetWorldRotationZ(float z)
{
	m_vVelocityRot.z += z - m_vWorldRot.z;

	m_vWorldRot.z = z;

	m_vRelativeRot.z = m_vWorldRot.z;

	if (m_pParent)
	{
		if (m_bInheritRotZ)
			m_vRelativeRot.z = m_vWorldRot.z - m_pParent->GetWorldRot().z;
	}

	Matrix	matRot;
	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_vWorldAxis[i].Normalize();
	}

	//m_matScale.Scaling(m_vWorldScale);
	m_bUpdateRot = true;

	// 자식이 있을경우 모든 자식을 반복하며 갱신된 Scale정보를 적용하게 한다.
	size_t	iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CUITransform::SetWorldPos(const Vector3& vPos)
{
	m_vVelocity += vPos - m_vWorldPos;

	m_vWorldPos = vPos;

	m_vRelativePos = m_vWorldPos;

	if (m_pParent)
	{
		Matrix	matRot;

		// 부모의 회전정보를 이용해서 회전행렬을 만들어준다.
		matRot.Rotation(m_pParent->GetWorldRot());

		// 회전의 중심점이 원점기준으로 되어 있으므로 부모의 위치를 기준으로
		// 회전할 수 있게 회전의 중심점을 변경한다.
		memcpy(&matRot._41, &m_pParent->GetWorldPos(), sizeof(Vector3));

		// 역행렬을 구해준다.
		matRot.Inverse();

		m_vRelativePos = m_vWorldPos.TransformCoord(matRot);
	}

	//m_matScale.Scaling(m_vWorldScale);
	m_bUpdatePos = true;

	// 자식이 있을경우 모든 자식을 반복하며 갱신된 Scale정보를 적용하게 한다.
	size_t	iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritPos();
	}
}

void CUITransform::SetWorldPos(float x, float y, float z)
{
	m_vVelocity += Vector3(x, y, z) - m_vWorldPos;

	m_vWorldPos = Vector3(x, y, z);

	m_vRelativePos = m_vWorldPos;

	if (m_pParent)
	{
		Matrix	matRot;

		// 부모의 회전정보를 이용해서 회전행렬을 만들어준다.
		matRot.Rotation(m_pParent->GetWorldRot());

		// 회전의 중심점이 원점기준으로 되어 있으므로 부모의 위치를 기준으로
		// 회전할 수 있게 회전의 중심점을 변경한다.
		memcpy(&matRot._41, &m_pParent->GetWorldPos(), sizeof(Vector3));

		// 역행렬을 구해준다.
		matRot.Inverse();

		m_vRelativePos = m_vWorldPos.TransformCoord(matRot);
	}

	//m_matScale.Scaling(m_vWorldScale);
	m_bUpdatePos = true;

	// 자식이 있을경우 모든 자식을 반복하며 갱신된 Scale정보를 적용하게 한다.
	size_t	iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritPos();
	}
}

void CUITransform::AddWorldScale(const Vector3& vScale)
{
	m_vVelocityScale += vScale;

	m_vWorldScale += vScale;

	m_vRelativeScale = m_vWorldScale;

	if (m_pParent && m_bInheritScale)
		m_vRelativeScale = m_vWorldScale / m_pParent->GetWorldScale();

	//m_matScale.Scaling(m_vWorldScale);
	m_bUpdateScale = true;

	// 자식이 있을경우 모든 자식을 반복하며 갱신된 Scale정보를 적용하게 한다.
	size_t	iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritScale();
	}
}

void CUITransform::AddWorldScale(float x, float y, float z)
{
	m_vVelocityScale += Vector3(x, y, z);

	m_vWorldScale += Vector3(x, y, z);

	m_vRelativeScale = m_vWorldScale;

	if (m_pParent && m_bInheritScale)
		m_vRelativeScale = m_vWorldScale / m_pParent->GetWorldScale();

	//m_matScale.Scaling(m_vWorldScale);
	m_bUpdateScale = true;

	// 자식이 있을경우 모든 자식을 반복하며 갱신된 Scale정보를 적용하게 한다.
	size_t	iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritScale();
	}
}

void CUITransform::AddWorldPos(const Vector3& vPos)
{
	m_vVelocity += vPos;

	m_vWorldPos += vPos;

	m_vRelativePos = m_vWorldPos;

	if (m_pParent)
	{
		Matrix	matRot;

		// 부모의 회전정보를 이용해서 회전행렬을 만들어준다.
		matRot.Rotation(m_pParent->GetWorldRot());

		// 회전의 중심점이 원점기준으로 되어 있으므로 부모의 위치를 기준으로
		// 회전할 수 있게 회전의 중심점을 변경한다.
		memcpy(&matRot._41, &m_pParent->GetWorldPos(), sizeof(Vector3));

		// 역행렬을 구해준다.
		matRot.Inverse();

		m_vRelativePos = m_vWorldPos.TransformCoord(matRot);
	}

	//m_matScale.Scaling(m_vWorldScale);
	m_bUpdatePos = true;

	// 자식이 있을경우 모든 자식을 반복하며 갱신된 Scale정보를 적용하게 한다.
	size_t	iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritPos();
	}
}

void CUITransform::AddWorldPos(float x, float y, float z)
{
	m_vVelocity += Vector3(x, y, z);

	m_vWorldPos += Vector3(x, y, z);

	m_vRelativePos = m_vWorldPos;

	if (m_pParent)
	{
		Matrix	matRot;

		// 부모의 회전정보를 이용해서 회전행렬을 만들어준다.
		matRot.Rotation(m_pParent->GetWorldRot());

		// 회전의 중심점이 원점기준으로 되어 있으므로 부모의 위치를 기준으로
		// 회전할 수 있게 회전의 중심점을 변경한다.
		memcpy(&matRot._41, &m_pParent->GetWorldPos(), sizeof(Vector3));

		// 역행렬을 구해준다.
		matRot.Inverse();

		m_vRelativePos = m_vWorldPos.TransformCoord(matRot);
	}

	//m_matScale.Scaling(m_vWorldScale);
	m_bUpdatePos = true;

	// 자식이 있을경우 모든 자식을 반복하며 갱신된 Scale정보를 적용하게 한다.
	size_t	iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritPos();
	}
}

void CUITransform::AddWorldRotation(const Vector3& vRot)
{
	m_vVelocityRot += vRot;

	m_vWorldRot += vRot;

	m_vRelativeRot = m_vWorldRot;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vRelativeRot.x = m_vWorldRot.x - m_pParent->GetWorldRot().x;

		if (m_bInheritRotY)
			m_vRelativeRot.y = m_vWorldRot.y - m_pParent->GetWorldRot().y;

		if (m_bInheritRotZ)
			m_vRelativeRot.z = m_vWorldRot.z - m_pParent->GetWorldRot().z;
	}

	Matrix	matRot;
	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_vWorldAxis[i].Normalize();
	}

	//m_matScale.Scaling(m_vWorldScale);
	m_bUpdateRot = true;

	// 자식이 있을경우 모든 자식을 반복하며 갱신된 Scale정보를 적용하게 한다.
	size_t	iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CUITransform::AddWorldRotation(float x, float y, float z)
{
	m_vVelocityRot += Vector3(x, y, z);

	m_vWorldRot += Vector3(x, y, z);

	m_vRelativeRot = m_vWorldRot;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vRelativeRot.x = m_vWorldRot.x - m_pParent->GetWorldRot().x;

		if (m_bInheritRotY)
			m_vRelativeRot.y = m_vWorldRot.y - m_pParent->GetWorldRot().y;

		if (m_bInheritRotZ)
			m_vRelativeRot.z = m_vWorldRot.z - m_pParent->GetWorldRot().z;
	}

	Matrix	matRot;
	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_vWorldAxis[i].Normalize();
	}

	//m_matScale.Scaling(m_vWorldScale);
	m_bUpdateRot = true;

	// 자식이 있을경우 모든 자식을 반복하며 갱신된 Scale정보를 적용하게 한다.
	size_t	iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CUITransform::AddWorldRotationX(float x)
{
	m_vVelocityRot.x += x;

	m_vWorldRot.x += x;

	m_vRelativeRot.x = m_vWorldRot.x;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vRelativeRot.x = m_vWorldRot.x - m_pParent->GetWorldRot().x;
	}

	Matrix	matRot;
	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_vWorldAxis[i].Normalize();
	}

	//m_matScale.Scaling(m_vWorldScale);
	m_bUpdateRot = true;

	// 자식이 있을경우 모든 자식을 반복하며 갱신된 Scale정보를 적용하게 한다.
	size_t	iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CUITransform::AddWorldRotationY(float y)
{
	m_vVelocityRot.y += y;

	m_vWorldRot.y += y;

	m_vRelativeRot.y = m_vWorldRot.y;

	if (m_pParent)
	{
		if (m_bInheritRotY)
			m_vRelativeRot.y = m_vWorldRot.y - m_pParent->GetWorldRot().y;
	}

	Matrix	matRot;
	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_vWorldAxis[i].Normalize();
	}

	//m_matScale.Scaling(m_vWorldScale);
	m_bUpdateRot = true;

	// 자식이 있을경우 모든 자식을 반복하며 갱신된 Scale정보를 적용하게 한다.
	size_t	iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CUITransform::AddWorldRotationZ(float z)
{
	m_vVelocityRot.z += z;

	m_vWorldRot.z += z;

	m_vRelativeRot.z = m_vWorldRot.z;

	if (m_pParent)
	{
		if (m_bInheritRotZ)
			m_vRelativeRot.z = m_vWorldRot.z - m_pParent->GetWorldRot().z;
	}

	Matrix	matRot;
	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_vWorldAxis[i].Normalize();
	}

	//m_matScale.Scaling(m_vWorldScale);
	m_bUpdateRot = true;

	// 자식이 있을경우 모든 자식을 반복하며 갱신된 Scale정보를 적용하게 한다.
	size_t	iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CUITransform::SetPivot(const Vector3& vPivot)
{
	m_vPivot = vPivot;
}

void CUITransform::SetPivot(float x, float y, float z)
{
	m_vPivot = Vector3(x, y, z);
}

void CUITransform::SetMeshSize(const Vector3& vSize)
{
	m_vMeshSize = vSize;
}
