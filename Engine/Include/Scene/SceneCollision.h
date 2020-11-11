#pragma once

#include "../GameEngine.h"

//2D,3D,UI�� Collision������ ���� ����ü
//----------------------------------
typedef struct _tagCollisionSectionInfo
{
	vector<class CCollisionSection*> vecSection;//Section���� ����

	Vector3		vSectionSize;	//Sectionũ��
	Vector3		vCenter;		//�߽�
	
	//x,y,z�ະ Section����
	//----------------------
	int			iCountX;
	int			iCountY;
	int			iCountZ;
	//----------------------

	_tagCollisionSectionInfo() :
		iCountX(0),
		iCountY(0),
		iCountZ(0)
	{
	}
}CollisionSectionInfo, * PCollisionSectionInfo;
//----------------------------------

class CSceneCollision
{
	friend class CScene;

private:
	CSceneCollision();
	~CSceneCollision();

private:
	//�� �������� Section�� ������ ����ü
	//------------------------------------
	PCollisionSectionInfo	m_pSection2D;
	PCollisionSectionInfo	m_pSection3D;
	//------------------------------------

	//���� ũ��
	Vector3		m_vWorldSize;

	//�ּ�, �ִ� ��ǥ
	Vector3		m_vMin;
	Vector3		m_vMax;

	//�浹ó���� UIControl
	vector<class CUIControl*>	m_vecUI;

	//���콺�� �浹�ߴ� Object
	class CCollider*			m_pPrevMouseCollider;
	class CUIControl*			m_pPrevMouseUI;

public:
	const vector<class CUIControl*>& GetUIVec() const
	{
		return m_vecUI;
	}

public:
	//Set�Լ�
	//------------------------------------------
	void SetWorldSize(const Vector3& vWorldSize)
	{
		m_vWorldSize = vWorldSize;
	}

	void SetCenter(const Vector3& vCenter);
	//------------------------------------------

public:
	bool Init();
	void AddCollider(class CCollider* pCollider);
	void AddUI(class CUIControl* pUI);
	void Collision(float fTime);

private:
	static bool SortUI(class CUIControl* pSrc, class CUIControl* pDest);
};


