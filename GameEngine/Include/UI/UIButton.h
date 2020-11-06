#pragma once

#include "UIControl.h"

enum BUTTON_SOUND
{
	SOUND_MOUSEON,
	SOUND_CLICK,
	SOUND_END
};

//버튼 상태에 따른 Texture,Color
//-------------------------------
typedef struct _tagButtonStateInfo
{
	class CTexture* pTexture;
	Vector4			vColor;

	_tagButtonStateInfo()
		: pTexture(nullptr),vColor(Vector4::White)
	{
	}
}ButtonStateInfo, * PButtonStateInfo;
//-------------------------------

class CUIButton : public CUIControl
{
	friend class CUIObject;

protected:
	CUIButton();
	CUIButton(const CUIButton& control);
	virtual ~CUIButton();

protected:
	Button_State		m_eState;
	vector<ButtonStateInfo>	m_vecButtonState;

	//버튼 상태
	//--------------------------
	bool				m_bDown;
	bool				m_bPush;
	bool				m_bUp;
	//--------------------------

	//클릭시 호출될 함수
	function<void()>	m_ClickCallback;

	vector<class CSound*>	m_vecSound;

public:
	//Texture Set함수
	void SetButtonStateTexture(Button_State eState, const string& strName);

	//Color Set함수
	void SetButtonStateColor(Button_State eState, const Vector4& vColor);
	void SetButtonStateColor(Button_State eState, BYTE r, BYTE g, BYTE b, BYTE a);
	
	//Sound Set함수
	void SetButtonSound(BUTTON_SOUND eSound, const string& strName);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CUIButton* Clone();

public:
	virtual void CollisionMouse(const Vector2& vMousePos, float fTime);
	virtual void CollisionReleaseMouse(const Vector2& vMousePos, float fTime);

public:
	template <typename T>
	void SetClickCallback(T* pObj, void(T::* pFunc)())
	{
		m_ClickCallback = bind(pFunc, pObj);
	}
};

