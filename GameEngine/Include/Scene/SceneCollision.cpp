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
    //각 공간에 할당된 Section이 존재하면 해제
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
	//월드의 가장 좌하단 좌표
	m_vMin = vCenter - m_vWorldSize / 2.f;

	//월드의 가장 우상단 좌표
	m_vMax = m_vMin + m_vWorldSize;

	m_pSection2D->vCenter = vCenter;

	//각 2D Section의 Min,Max Setting
	//----------------------------------
	size_t iSize = m_pSection2D->vecSection.size();
	for (size_t iCnt = 0; iCnt < iSize; ++iCnt)
	{
		Vector3 vMin, vMax;

		//(m_pSection2D->iCountX % 2) = Section의 X축 개수가 홀수면 반만큼 추가
		//(m_pSection2D->vSectionSize.x / 2.f) = Section크기의 반
		//중심을 월점으로 기준으로한 Section들의 Min,Max를 Setting
		//나중에 사용할 때 Check
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

	//해당 Collider가 World내에 있는지 확인
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

	//Collider 원점을 기준으로 Min,Max Setting
	//------------------------------------
	Vector3	vMinPos = pCollider->GetMin() - m_vMin;
	Vector3	vMaxPos = pCollider->GetMax() - m_vMin;
	//------------------------------------

	//Collider가 속한 Section Index Search
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

	//해당되는 모든 Section에 AddCollider 호출
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
	//마우스, UI 충돌처리
	//----------------------------------------------
	
	//ZOrder가 큰 순으로 정렬
	sort(m_vecUI.begin(), m_vecUI.end(), CSceneCollision::SortUI);


	//savepoint 여기서 문제가 INT_MIN으로 했는데 비교에서 0이 더 작게 나옴
	//둘다 INT_MIN이여서 그럼

	Vector2 vMousePos = GET_SINGLE(CInput)->GetMousePos();
	size_t iSize = m_vecUI.size();

	bool bIsInUI = false;

	for (size_t iCnt = 0; iCnt < iSize; ++iCnt)
	{
		//충돌한 경우
		//-------------------------------------
		if (m_vecUI[iCnt]->UIInMouse(vMousePos))
		{
			bIsInUI = true;

			//이전에 충돌했던 UI와 다른 경우 충돌 처리
			//----------------------------------------------
			if (m_pPrevMouseUI != m_vecUI[iCnt])
			{
				m_vecUI[iCnt]->CollisionMouse(vMousePos, fTime);

				//이전에 충돌했던 UI는 Release
				if (m_pPrevMouseUI)
					m_pPrevMouseUI->CollisionReleaseMouse(vMousePos, fTime);
			}
			//----------------------------------------------
			
			m_pPrevMouseUI = m_vecUI[iCnt];

			//이전에 충돌했던 Collider가 있으면 Release
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

	//마우스가 UI와 충돌하지 않은 경우 Collider와 충돌처리
	//----------------------------------------
	bool bWorldCollision = false;
	if (!bIsInUI)
	{
		//전에 충돌한 UI가 있으면 Release
		//-------------------------------------
		if (m_pPrevMouseUI)
		{
			m_pPrevMouseUI->CollisionReleaseMouse(vMousePos, fTime);
			m_pPrevMouseUI = nullptr;
		}
		//-------------------------------------

		//마우스가 존재하는 Section Setting
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
