#include "CollisionSection.h"
#include "../Component/Collider.h"

CCollisionSection::CCollisionSection()
{
    m_vecCollider.reserve(300);
}

CCollisionSection::~CCollisionSection()
{
}

bool CCollisionSection::Init(int idxX, int idxY, int idxZ, int iIndex, const Vector3& vSectionSize, const Vector3& vWorldSize)
{
	m_iIndexX = idxX;
	m_iIndexY = idxY;
	m_iIndexZ = idxZ;
	m_iIndex = iIndex;
	m_vSectionSize = vSectionSize;
	m_vWorldSize = vWorldSize;

	return true;
}

void CCollisionSection::SetSection(const Vector3& vMin, const Vector3& vMax)
{
	m_vMin = vMin;
	m_vMax = vMax;
}

void CCollisionSection::AddCollider(CCollider* pCollider)
{
	m_vecCollider.push_back(pCollider);

	pCollider->AddSection(m_iIndex);
}

void CCollisionSection::Clear()
{
	size_t	iSize = m_vecCollider.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecCollider[i]->Clear();
	}

	m_vecCollider.clear();
}

void CCollisionSection::Collision(float fTime)
{
	//해당 Section에 존재하는 Collider들간 충돌 처리

	//해당 Section에 존재하는 Collider를 순회하여
	//서로 떨어지게된 Collider를 처리
	//------------------------------------------
	size_t iSize = m_vecCollider.size();
	for (size_t iCnt = 0; iCnt < iSize; ++iCnt)
	{
		//이번 프레임에서 이미 처리됐으면 pass
		if (m_vecCollider[iCnt]->IsCurrentFrameSectionCheck())
			continue;

		m_vecCollider[iCnt]->CurrentFrameSectionCheck();
		m_vecCollider[iCnt]->CheckPrevColliderSection(fTime);
	}
	//------------------------------------------

	//개수가 2개 미만인경우는 충돌 처리할 필요 X
	if (iSize < 2)
		return;

	//충돌 Check
	//------------------------------------------
	size_t iFrontSize = iSize - 1;
	for (size_t iFrontIdx = 0; iFrontIdx < iFrontSize; ++iFrontIdx)
	{
		CCollider* pSrc = m_vecCollider[iFrontIdx];
		
		for (size_t iBackIdx = iFrontIdx + 1; iBackIdx < iSize; ++iBackIdx)
		{
			CCollider* pDest = m_vecCollider[iBackIdx];

			//다른 Section에서 이미 충돌처리가 된 경우 pass
			//-------------------------------------------
			if (pSrc->CheckCurrentCollision(pDest))
				continue;
			//-------------------------------------------

			//Profile을 비교하여 서로 무시하는 경우 pass
			//-------------------------------------------
			PCollisionProfile pSrcProfile = pSrc->GetCollisionProfile();
			PCollisionProfile pDestProfile = pDest->GetCollisionProfile();

			if (pSrcProfile->vecChannel[(int)pDestProfile->eChannel].eType ==
				Collision_Type::Ignore ||
				pDestProfile->vecChannel[(int)pSrcProfile->eChannel].eType ==
				Collision_Type::Ignore)
				continue;
			//-------------------------------------------

			if (pSrc->Collision(pDest))
			{
				//처음 충돌한 경우
				//----------------------------------
				if (!pSrc->CheckPrevCollision(pDest))
				{
					pSrc->AddPrevCollider(pDest);
					pDest->AddPrevCollider(pSrc);

					pSrc->CallCollisionCallback(Collision_State::Begin, pDest, fTime);
					pDest->CallCollisionCallback(Collision_State::Begin, pSrc, fTime);
				}
				//----------------------------------				
			}

			//충돌 상태에서 떨어지는 상태로 바뀐 경우
			//----------------------------------		
			else if (pSrc->CheckPrevCollision(pDest))
			{
				pSrc->DeletePrevCollider(pDest);
				pDest->DeletePrevCollider(pSrc);

				pSrc->CallCollisionCallback(Collision_State::End, pDest, fTime);
				pDest->CallCollisionCallback(Collision_State::End, pDest, fTime);
			}
			//----------------------------------		
		}
	}
	//------------------------------------------
}

CCollider* CCollisionSection::CollisionMouse(const Vector2& vMouse)
{
	size_t	iSize = m_vecCollider.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		if (m_vecCollider[i]->CollisionMouse(vMouse))
		{
			m_vecCollider[i]->SetMouseCollision(true);
			return m_vecCollider[i];
		}
	}

	return nullptr;
}
