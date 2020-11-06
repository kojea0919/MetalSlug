#pragma once

#include "UIControl.h"

//UISpriteInfo 구조체
//---------------------------------------------
typedef struct _tagUISpriteInfo
{
	class CAnimation2DSequence* pAnimation;
	float			fPlayRate;
	float			fPlayTime;
	int				iFrame;
	bool			bLoop;

	_tagUISpriteInfo()
		: fPlayRate(1.f),fPlayTime(0.f),
		iFrame(0),bLoop(true)
	{}
}UISpriteInfo, *PUISpriteInfo;
//---------------------------------------------


class CUISprite : public CUIControl
{
	friend class CUIObject;

protected:
	CUISprite();
	CUISprite(const CUISprite& sprite);
	virtual ~CUISprite();

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CUISprite* Clone();

public:
	//Sprite관리 함수
	//-------------------------------------
	void AddSpriteInfo(const string& strName,
		const string& strAnimName,
		bool bLoop, float fPlayRate = 1.f);
	void AddSpriteInfo(const string& strName,
		class CAnimation2DSequence* pSequence,
		bool bLoop, float fPlayRate = 1.f);
	void AddSpriteInfo(const string& strName,
		const string& strSequenceName);
	void ChangeSprite(const string& strName);
	//-------------------------------------

private:
	PUISpriteInfo FindSpriteInfo(const string& strName);

protected:
	unordered_map<string, PUISpriteInfo> m_mapSpriteInfo;

	PUISpriteInfo	m_pCurrent;

	SpriteCBuffer	m_tCBuffer;
};


