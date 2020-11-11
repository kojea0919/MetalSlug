#pragma once

#include "../GameEngine.h"

//2D,3D,UI별 Collision정보를 담을 구조체
//----------------------------------
typedef struct _tagCollisionSectionInfo
{
	vector<class CCollisionSection*> vecSection;//Section관리 벡터

	Vector3		vSectionSize;	//Section크기
	Vector3		vCenter;		//중심
	
	//x,y,z축별 Section개수
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
	//각 공간에서 Section을 관리할 구조체
	//------------------------------------
	PCollisionSectionInfo	m_pSection2D;
	PCollisionSectionInfo	m_pSection3D;
	//------------------------------------

	//월드 크기
	Vector3		m_vWorldSize;

	//최소, 최대 좌표
	Vector3		m_vMin;
	Vector3		m_vMax;

	//충돌처리할 UIControl
	vector<class CUIControl*>	m_vecUI;

	//마우스와 충돌했던 Object
	class CCollider*			m_pPrevMouseCollider;
	class CUIControl*			m_pPrevMouseUI;

public:
	const vector<class CUIControl*>& GetUIVec() const
	{
		return m_vecUI;
	}

public:
	//Set함수
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


