#pragma once

#include "../Ref.h"

class CUIObject : public CRef
{
	friend class CUIManager;
	friend class CUIViewport;
	friend class CScene;

protected:
	CUIObject();
	CUIObject(const CUIObject& obj);
	virtual ~CUIObject();

protected:
	class CUIViewport*	m_pViewport;
	class CUIControl*	m_pRoot;
	class CScene*		m_pScene;
	int					m_iZOrder;

	vector<CUIControl*>	m_vecControl;

public:
	int GetZOrder() const
	{
		return m_iZOrder;
	}

	void SetZOrder(int iZOrder)
	{
		m_iZOrder = iZOrder;
	}

	//해당 위치와 UIContorl이 충돌하는지 Check하는 함수
	class CUIControl * UIInMouse(const Vector2& vMousePos);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CUIObject* Clone();

public:
	void SetRoot(class CUIControl* pRoot);
	class CUIControl* FindUIControl(const string& strName);
	void AddToViewport();

	//WidgetComponent사용시 Z값을 사용하므로 z값을 Setting
	void SetWidgetZ(float fZ);

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
	template <typename T>
	T* CreateControl(const string& strName)
	{
		T* pControl = new T;

		pControl->SetName(strName);
		pControl->m_pViewport = m_pViewport;
		pControl->m_pOwner = this;
		pControl->m_pScene = m_pScene;

		if (!pControl->Init())
		{
			SAFE_RELEASE(pControl);
			return nullptr;
		}

		m_vecControl.push_back(pControl);

		return pControl;
	}

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

private:
	static bool SortControl(class CUIControl* pSrc, class CUIControl* pDest);
};

