#pragma once

#include "../Ref.h"

class CUIControl : public CRef
{
	friend class CUIObject;

protected:
	CUIControl();
	CUIControl(const CUIControl& control);
	virtual ~CUIControl();

protected:
	bool				m_bStart;
	class CUIObject*	m_pOwner;
	class CUITransform* m_pTransform;
	class CUIViewport*	m_pViewport;
	class CScene*		m_pScene;

	//계층 구조
	//-----------------------------
	CUIControl*			m_pParent;
	vector<CUIControl*>	m_vecChild;
	//-----------------------------

	//사용 Resource
	//-----------------------------
	class CMesh2D*		m_pBodyMesh;
	class CTexture*		m_pUITexture;
	class CMaterial*	m_pMaterial;
	//-----------------------------

	int					m_iZOrder;

	Vector3				m_vMin;
	Vector3				m_vMax;

public:
	Vector3 GetMin() const
	{
		return m_vMin;
	}

	Vector3 GetMax() const
	{
		return m_vMax;
	}
	
	int GetZOrder() const;

	void SetZOrder(int iZOrder)
	{
		m_iZOrder = iZOrder;
	}

	bool IsStart() const
	{
		return m_bStart;
	}

	class CUITransform* GetTransform() const
	{
		return m_pTransform;
	}

	string GetParentName() const
	{
		if (m_pParent)
			return "";

		return m_pParent->GetName();
	}

public:
	void AddChild(CUIControl* pChild, const string& strSocketName = "");
	void DeleteChild(CUIControl* pChild);
	CUIControl* FindControl(const string& strName);
	void GetAllControlName(vector<HierarchyName>& vecName);
	void GetAllControl(vector<CUIControl*>& vecSceneCom);
	bool SetTexture(const string& strName);
	bool SetTexture(class CTexture* pTexture);

private:
	void DetatchChild(CUIControl* pChild);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CUIControl* Clone();

public:
	//충돌된 경우 호출할 함수
	virtual void CollisionMouse(const Vector2& vMousePos, float fTime);

	//출돌이 끝난 경우 호출할 함수
	virtual void CollisionReleaseMouse(const Vector2& vMousePos, float fTime);

	//해당 위치와 UIContorl이 충돌하는지 Check하는 함수
	bool UIInMouse(const Vector2& vMousePos);

public:
	Vector3 GetVelocityScale()	const;
	Vector3 GetVelocityRot()	const;
	Vector3 GetVelocity()	const;
	float GetCurrentVelocity()	const;

public:
	void SetInheritScale(bool bInherit);
	void SetInheritRotX(bool bInherit);
	void SetInheritRotY(bool bInherit);
	void SetInheritRotZ(bool bInherit);
	void InheritScale();
	void InheritRot();
	void InheritPos();

public:
	void SetRelativeScale(const Vector3& vScale);
	void SetRelativeScale(float x, float y, float z);
	void SetRelativeRotation(const Vector3& vRot);
	void SetRelativeRotation(float x, float y, float z);
	void SetRelativeRotationX(float x);
	void SetRelativeRotationY(float y);
	void SetRelativeRotationZ(float z);
	void SetRelativePos(const Vector3& vPos);
	void SetRelativePos(float x, float y, float z);
	void AddRelativeScale(const Vector3& vScale);
	void AddRelativeScale(float x, float y, float z);
	void AddRelativePos(const Vector3& vPos);
	void AddRelativePos(float x, float y, float z);
	void AddRelativeRotation(const Vector3& vRot);
	void AddRelativeRotation(float x, float y, float z);
	void AddRelativeRotationX(float x);
	void AddRelativeRotationY(float y);
	void AddRelativeRotationZ(float z);

public:
	Vector3 GetRelativeScale()	const;
	Vector3 GetRelativeRot()	const;
	Vector3 GetRelativePos()	const;
	Vector3 GetRelativeAxis(AXIS eAxis)	const;

public:
	void SetWorldScale(const Vector3& vScale);
	void SetWorldScale(float x, float y, float z);
	void SetWorldRotation(const Vector3& vRot);
	void SetWorldRotation(float x, float y, float z);
	void SetWorldRotationX(float x);
	void SetWorldRotationY(float y);
	void SetWorldRotationZ(float z);
	void SetWorldPos(const Vector3& vPos);
	void SetWorldPos(float x, float y, float z);
	void AddWorldScale(const Vector3& vScale);
	void AddWorldScale(float x, float y, float z);
	void AddWorldPos(const Vector3& vPos);
	void AddWorldPos(float x, float y, float z);
	void AddWorldRotation(const Vector3& vRot);
	void AddWorldRotation(float x, float y, float z);
	void AddWorldRotationX(float x);
	void AddWorldRotationY(float y);
	void AddWorldRotationZ(float z);
	void SetPivot(const Vector3& vPivot);
	void SetPivot(float x, float y, float z);
	void SetMeshSize(const Vector3& vSize);

public:
	Vector3 GetWorldScale()	const;
	Vector3 GetWorldRot()	const;
	Vector3 GetWorldPos()	const;
	Vector3 GetWorldAxis(AXIS eAxis)	const;
	Vector3 GetPivot()	const;

	Matrix GetScaleMatrix()	const;
	Matrix GetRotMatrix()	const;
	Matrix GetTranslationMatrix()	const;
	Matrix GetWorldMatrix()	const;

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

