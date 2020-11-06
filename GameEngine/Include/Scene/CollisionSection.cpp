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
	//�ش� Section�� �����ϴ� Collider�鰣 �浹 ó��

	//�ش� Section�� �����ϴ� Collider�� ��ȸ�Ͽ�
	//���� �������Ե� Collider�� ó��
	//------------------------------------------
	size_t iSize = m_vecCollider.size();
	for (size_t iCnt = 0; iCnt < iSize; ++iCnt)
	{
		//�̹� �����ӿ��� �̹� ó�������� pass
		if (m_vecCollider[iCnt]->IsCurrentFrameSectionCheck())
			continue;

		m_vecCollider[iCnt]->CurrentFrameSectionCheck();
		m_vecCollider[iCnt]->CheckPrevColliderSection(fTime);
	}
	//------------------------------------------

	//������ 2�� �̸��ΰ��� �浹 ó���� �ʿ� X
	if (iSize < 2)
		return;

	//�浹 Check
	//------------------------------------------
	size_t iFrontSize = iSize - 1;
	for (size_t iFrontIdx = 0; iFrontIdx < iFrontSize; ++iFrontIdx)
	{
		CCollider* pSrc = m_vecCollider[iFrontIdx];
		
		for (size_t iBackIdx = iFrontIdx + 1; iBackIdx < iSize; ++iBackIdx)
		{
			CCollider* pDest = m_vecCollider[iBackIdx];

			//�ٸ� Section���� �̹� �浹ó���� �� ��� pass
			//-------------------------------------------
			if (pSrc->CheckCurrentCollision(pDest))
				continue;
			//-------------------------------------------

			//Profile�� ���Ͽ� ���� �����ϴ� ��� pass
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
				//ó�� �浹�� ���
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

			//�浹 ���¿��� �������� ���·� �ٲ� ���
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
