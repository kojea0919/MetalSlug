#pragma once

#include "Ref.h"
#include "InputObj.h"

class CGameObject : public CRef
{
	friend class CScene;
	friend class CInput;

protected:
	CGameObject();
	CGameObject(const CGameObject & obj);
	virtual ~CGameObject();

protected:
	//������Ʈ�� ���� Scene
	class CScene* m_pScene;

	//������Ʈ�� Start�Լ� ȣ�� ����
	bool m_bStart;

public:
	bool IsStart() const
	{
		return m_bStart;
	}

protected:
	//������Ʈ Ÿ��(Save,Load�� ���� � Object���� �˱����ؼ�)
	int					m_iObjType;

	//Component
	//---------------------------------------------------
	class CSceneComponent*				m_pRootComponent;
	vector<class CObjectComponent*>		m_vecObjComponent;
	//---------------------------------------------------

	//������Ʈ�� �ı��� �� Editor�� Worldâ������ �����ϱ����ؼ�
	//CWorldObjectDlg::DestroyEditorObject�Լ��� callback�Լ��� bind
	function<void(CGameObject*)>		m_DestroyEditorFunc;

	//�Է� �߰� Obj
	CInputObj* m_pInputObj;

public:
	//������Ʈ Ÿ�� Set,Get�Լ�
	//---------------------------
	void SetObjType(int iObjType)
	{
		m_iObjType;
	}
	
	int GetObjType() const
	{
		return m_iObjType;
	}
	//---------------------------


	//test code
	//---------------------------
//public:
//	class CCamera* m_pCamera;
//
//public:
//	void SetCamera(class CCamera* pCamera)
//	{
//		m_pCamera = pCamera;
//	}
	//---------------------------

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PrevRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);

public:
	//Root Component Get,Set�Լ�
	//-------------------------------------------------
	void SetRootComponent(class CSceneComponent* pRoot);
	class CSceneComponent* GetRootComponent();
	//-------------------------------------------------

	//Component Ž�� �Լ�
	//-------------------------------------------------
	class CSceneComponent* FindSceneComponent(const string& strName);
	class CObjectComponent* FindObjectComponent(const string& strName);
	//-------------------------------------------------

	//��� Component �̸��� vector�� �����Ͽ� ��ȯ
	//-------------------------------------------------
	void GetAllSceneComponentName(vector<HierarchyName>& vecName);
	void GetAllObjectComponentName(vector<string>& vecName);
	//-------------------------------------------------

public:
	virtual void CallAnimation2DNotify(const string& strName);

public:
	//Velocity Get�Լ�
	//----------------------------
	Vector3 GetVelocityScale() const;
	Vector3 GetVelocityRot() const;
	Vector3 GetVelocity() const;
	float GetCurrentVelocity() const;
	//----------------------------

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
	//Component ���� �Լ�
	//----------------------------------------
	template<typename T>
	T* CreateComponent(const string& strName)
	{
		T* pComponent = new T;

		//Component ���� setting
		//-----------------------------
		pComponent->SetName(strName);
		pComponent->m_pScene = m_pScene;
		pComponent->m_pObject = this;
		//-----------------------------

		if (!pComponent->Init())
		{
			SAFE_RELEASE(pComponent);
			return nullptr;
		}

		if (pComponent->GetComponentType() == COMPONENT_TYPE::OBJECT)
		{
			pComponent->AddRef();
			m_vecObjComponent.push_back((CObjectComponent*)pComponent);
		}
		return pComponent;
	}
	//----------------------------------------

	//Editor���� ������Ʈ �ı��� WorldDlgâ���� �������� �Լ� ���
	//----------------------------------------
	template<typename T>
	void SetDestroyEditorFunction(T* pObj, void(T::* pFunc)(CGameObject*))
	{
		m_DestroyEditorFunc = bind(pFunc, pObj, placeholders::_1);
	}
	//----------------------------------------

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	//BindAxis ���
	template<typename T>
	void AddBindAxis(const string& strName, T* pObj, void (T::* pFunc)(float, float))
	{
		m_pInputObj->AddBindAxis<T>(strName, pObj, pFunc);
	}

	//BindAction ���
	template<typename T>
	void AddBindAction(const string& strName, KEY_TYPE eType, T* pObj, void (T::* pFunc)(float))
	{
		m_pInputObj->AddBindAction<T>(strName, eType, pObj, pFunc);
	}
};