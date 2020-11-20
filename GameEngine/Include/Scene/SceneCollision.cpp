#include "SceneCollision.h"
#include "CollisionSection.h"
#include "../Device.h"
#include "../Component/Collider.h"
#include "../UI/UIControl.h"
#include "../Input.h"

CSceneCollision::CSceneCollision()
	: m_pPrevMouseCollider(nullptr), m_pPrevMouseUI(nullptr)
{
	m_vecUI.reserve(500);
}

CSceneCollision::~CSceneCollision()
{
    //�� ������ �Ҵ�� Section�� �����ϸ� ����
    //--------------------------------------
	if (m_pSection2D)
	{
		size_t	iSize = m_pSection2D->vecSection.size();

		for (size_t i = 0; i < iSize; ++i)
		{
			SAFE_DELETE(m_pSection2D->vecSection[i]);
		}

		SAFE_DELETE(m_pSection2D);
	}


	if (m_pSection3D)
	{
		size_t iSize = m_pSection3D->vecSection.size();

		for (size_t i = 0; i < iSize; ++i)
		{
			SAFE_DELETE(m_pSection3D->vecSection[i]);
		}

		SAFE_DELETE(m_pSection3D);
	}
    //--------------------------------------
}

void CSceneCollision::SetCenter(const Vector3& vCenter)
{
	//������ ���� ���ϴ� ��ǥ
	m_vMin = vCenter - m_vWorldSize / 2.f;

	//������ ���� ���� ��ǥ
	m_vMax = m_vMin + m_vWorldSize;

	m_pSection2D->vCenter = vCenter;

	//�� 2D Section�� Min,Max Setting
	//----------------------------------
	size_t iSize = m_pSection2D->vecSection.size();
	for (size_t iCnt = 0; iCnt < iSize; ++iCnt)
	{
		Vector3 vMin, vMax;

		//(m_pSection2D->iCountX % 2) = Section�� X�� ������ Ȧ���� �ݸ�ŭ �߰�
		//(m_pSection2D->vSectionSize.x / 2.f) = Sectionũ���� ��
		//�߽��� �������� ���������� Section���� Min,Max�� Setting
		//���߿� ����� �� Check
		vMin.x = (m_pSection2D->iCountX % 2) * (m_pSection2D->vSectionSize.x / 2.f) -
			((m_pSection2D->iCountX / 2) - m_pSection2D->vecSection[iCnt]->GetIndexX()) * m_pSection2D->vSectionSize.x;

		vMin.y = (m_pSection2D->iCountY % 2) * (m_pSection2D->vSectionSize.y / 2.f) -
			((m_pSection2D->iCountY / 2) - m_pSection2D->vecSection[iCnt]->GetIndexY()) * m_pSection2D->vSectionSize.y;

		vMin.z = 0.f;

		vMax = vMin + m_pSection2D->vSectionSize;

		m_pSection2D->vecSection[iCnt]->SetSection(vMin, vMax);
	}
	//----------------------------------

	//3D
	//----------------------------------
	iSize = m_pSection3D->vecSection.size();
	for (size_t i = 0; i < iSize; ++i)
	{
		Vector3	vMin, vMax;
		vMin.x = (m_pSection3D->iCountX % 2) * (m_pSection3D->vSectionSize.x / 2.f) -
			((m_pSection3D->iCountX / 2) - m_pSection3D->vecSection[i]->GetIndexX()) * m_pSection3D->vSectionSize.x;

		vMin.y = (m_pSection3D->iCountY % 2) * (m_pSection3D->vSectionSize.y / 2.f) -
			((m_pSection3D->iCountY / 2) - m_pSection3D->vecSection[i]->GetIndexY()) * m_pSection3D->vSectionSize.y;

		vMin.z = (m_pSection3D->iCountZ % 2) * (m_pSection3D->vSectionSize.z / 2.f) -
			((m_pSection3D->iCountZ / 2) - m_pSection3D->vecSection[i]->GetIndexZ()) * m_pSection3D->vSectionSize.z;

		vMax = vMin + m_pSection3D->vSectionSize;

		m_pSection3D->vecSection[i]->SetSection(vMin, vMax);
	}
	//----------------------------------
}

bool CSceneCollision::Init()
{
	m_vWorldSize = Vector3(10000.f, 10000.f, 10000.f);

	m_pSection2D = new CollisionSectionInfo;
	m_pSection2D->vecSection.push_back(new CCollisionSection);
	m_pSection2D->iCountX = 1;
	m_pSection2D->iCountY = 1;
	m_pSection2D->iCountZ = 1;
	m_pSection2D->vSectionSize = Vector3(10000.f, 10000.f, 10000.f);
	m_pSection2D->vecSection[0]->Init(0,0,0,0, m_pSection2D->vSectionSize,
		m_vWorldSize);

	m_pSection3D = new CollisionSectionInfo;
	m_pSection3D->vecSection.push_back(new CCollisionSection);
	m_pSection3D->iCountX = 1;
	m_pSection3D->iCountY = 1;
	m_pSection3D->iCountZ = 1;
	m_pSection3D->vSectionSize = Vector3(10000.f, 10000.f, 10000.f);
	m_pSection3D->vecSection[0]->Init(0, 0, 0, 0, m_pSection3D->vSectionSize,
		m_vWorldSize);

    return true;
}

void CSceneCollision::AddCollider(CCollider* pCollider)
{
	Vector3 vMin = pCollider->GetMin();
	Vector3 vMax = pCollider->GetMax();

	//�ش� Collider�� World���� �ִ��� Ȯ��
	//------------------------------------
	if (vMin.x > m_vMax.x)
		return;

	else if (vMax.x < m_vMin.x)
		return;

	else if (vMin.y > m_vMax.y)
		return;

	else if (vMax.y < m_vMin.y)
		return;

	else if (vMin.z > m_vMax.z)
		return;

	else if (vMax.z < m_vMin.z)
		return;
	//------------------------------------

	//Collider ������ �������� Min,Max Setting
	//------------------------------------
	Vector3	vMinPos = pCollider->GetMin() - m_vMin;
	Vector3	vMaxPos = pCollider->GetMax() - m_vMin;
	//------------------------------------

	//Collider�� ���� Section Index Search
	//---------------------------------------
	
	PCollisionSectionInfo pInfo = m_pSection3D;

	if (pCollider->Is2D())
		pInfo = m_pSection2D;

	int	iIndexMinX, iIndexMinY, iIndexMinZ;
	int	iIndexMaxX, iIndexMaxY, iIndexMaxZ;
	iIndexMinX = (int)(vMinPos.x / pInfo->vSectionSize.x);
	iIndexMinY = (int)(vMinPos.y / pInfo->vSectionSize.y);
	iIndexMinZ = (int)(vMinPos.z / pInfo->vSectionSize.z);

	iIndexMaxX = (int)(vMaxPos.x / pInfo->vSectionSize.x);
	iIndexMaxY = (int)(vMaxPos.y / pInfo->vSectionSize.y);
	iIndexMaxZ = (int)(vMaxPos.z / pInfo->vSectionSize.z);

	//�ش�Ǵ� ��� Section�� AddCollider ȣ��
	//----------------------------------------------------------
	for (int z = iIndexMinZ; z <= iIndexMaxZ; ++z)
	{
		for (int y = iIndexMinY; y <= iIndexMaxY; ++y)
		{
			for (int x = iIndexMinX; x <= iIndexMaxX; ++x)
			{
				int	iIndex = z * (pInfo->iCountX * pInfo->iCountY) +
					y * pInfo->iCountX + x;

				if (iIndex < 0 || iIndex >= pInfo->vecSection.size())
					continue;

				pInfo->vecSection[iIndex]->AddCollider(pCollider);
			}
		}
	}
	//----------------------------------------------------------

}

void CSceneCollision::AddUI(CUIControl* pUI)
{
	m_vecUI.push_back(pUI);
}

void CSceneCollision::Collision(float fTime)
{
	//���콺, UI �浹ó��
	//----------------------------------------------
	
	//ZOrder�� ū ������ ����
	sort(m_vecUI.begin(), m_vecUI.end(), CSceneCollision::SortUI);


	//savepoint ���⼭ ������ INT_MIN���� �ߴµ� �񱳿��� 0�� �� �۰� ����
	//�Ѵ� INT_MIN�̿��� �׷�

	Vector2 vMousePos = GET_SINGLE(CInput)->GetMousePos();
	size_t iSize = m_vecUI.size();

	bool bIsInUI = false;

	for (size_t iCnt = 0; iCnt < iSize; ++iCnt)
	{
		//�浹�� ���
		//-------------------------------------
		if (m_vecUI[iCnt]->UIInMouse(vMousePos))
		{
			bIsInUI = true;

			//������ �浹�ߴ� UI�� �ٸ� ��� �浹 ó��
			//----------------------------------------------
			if (m_pPrevMouseUI != m_vecUI[iCnt])
			{
				m_vecUI[iCnt]->CollisionMouse(vMousePos, fTime);

				//������ �浹�ߴ� UI�� Release
				if (m_pPrevMouseUI)
					m_pPrevMouseUI->CollisionReleaseMouse(vMousePos, fTime);
			}
			//----------------------------------------------
			
			m_pPrevMouseUI = m_vecUI[iCnt];

			//������ �浹�ߴ� Collider�� ������ Release
			//----------------------------------------------
			if (m_pPrevMouseCollider)
			{
				m_pPrevMouseCollider->SetMouseCollision(false);
				m_pPrevMouseCollider->CallMouseCollisionCallback(Collision_State::End, vMousePos, fTime);
				m_pPrevMouseCollider = nullptr;
			}
			//----------------------------------------------

			break;
		}
		//-------------------------------------
	}

	//���콺�� UI�� �浹���� ���� ��� Collider�� �浹ó��
	//----------------------------------------
	bool bWorldCollision = false;
	if (!bIsInUI)
	{
		//���� �浹�� UI�� ������ Release
		//-------------------------------------
		if (m_pPrevMouseUI)
		{
			m_pPrevMouseUI->CollisionReleaseMouse(vMousePos, fTime);
			m_pPrevMouseUI = nullptr;
		}
		//-------------------------------------

		//���콺�� �����ϴ� Section Setting
		int iIndexX, iIndexY;
		iIndexX = (int)(vMousePos.x / m_pSection2D->vSectionSize.x);
		iIndexY = (int)(vMousePos.y / m_pSection2D->vSectionSize.y);

		if (iIndexX >= 0 && iIndexX < m_pSection2D->iCountX &&
			iIndexY >= 0 && iIndexY < m_pSection2D->iCountY)
		{
			int iIndex = iIndexY * m_pSection2D->iCountX + iIndexX;

			CCollider* pMouseCollision = m_pSection2D->vecSection[iIndex]->CollisionMouse(vMousePos);

			if (pMouseCollision)
			{
				bWorldCollision = true;
			}
		}
	}
	//----------------------------------------

	if (!bIsInUI && !bWorldCollision)
	{
		if (m_pPrevMouseUI)
		{
			m_pPrevMouseUI->CollisionReleaseMouse(vMousePos, fTime);
			m_pPrevMouseUI = nullptr;
		}

		if (m_pPrevMouseCollider)
		{
			m_pPrevMouseCollider->SetMouseCollision(false);
			m_pPrevMouseCollider->CallMouseCollisionCallback(Collision_State::End, vMousePos, fTime);
			m_pPrevMouseCollider = nullptr;
		}
	}

	//----------------------------------------------

	iSize = m_pSection2D->vecSection.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_pSection2D->vecSection[i]->Collision(fTime);
	}

	iSize = m_pSection2D->vecSection.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_pSection2D->vecSection[i]->Clear();
	}

	iSize = m_pSection3D->vecSection.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_pSection3D->vecSection[i]->Clear();
	}

	m_vecUI.clear();
}

bool CSceneCollision::SortUI(CUIControl* pSrc, CUIControl* pDest)
{
	return pSrc->GetZOrder() < pDest->GetZOrder();
}
