#pragma once
#include "../Ref.h"
#include "Animation2DNotify.h"

//Anim2DFrame구조체
//Atlas이미지 사용시 각 애니메이션의 시작 좌표와 끝 좌표 정보
//------------------------------------------------------------
typedef struct _tagAnim2DFrame
{
	Vector2 vStart;
	Vector2 vEnd;
}Anim2DFrame, *PAnim2DFrame;
//------------------------------------------------------------

class CAnimation2DSequence : public CRef
{
	friend class CResourceManager;
	friend class CSceneResource;
	friend class CSpriteComponent;
	friend class CUISprite;

private:
	CAnimation2DSequence();
	~CAnimation2DSequence();
	
private:
	IMAGE_TYPE		m_eImageType;
	class CTexture* m_pTexture;
	float			m_fPlayRate;
	float			m_fPlayTime;
	float			m_fPlayTimeMax;
	int				m_iFrame;
	int				m_iFrameMax;
	bool			m_bLoop;

	//이미지 cut 정보
	vector<Anim2DFrame>	m_vecAnimFrame;

	//이미지 pivot정보
	vector<Vector2> m_vecAnimPivot;

	//이미지 상대 위치 정보
	//-------------------------------------
	bool			m_bUseRelativePos;

	//해당 SpriteComponent에 자식으로 존재하는 SpriteComponent들의
	//RelativePos를 관리하는 Map
	unordered_map<string, vector<vector<Vector2>>*> m_mapChildAnimPos;
	//-------------------------------------

	//애니메이션을 가지고 있는 SpriteComponentList
	list<class CSpriteComponent*>	m_SpriteList;

	//애니메이션에 등록된 NotifyList
	list<CAnimation2DNotify*>		m_NotifyList;


public:
	bool GetTargetFramePosInfo(const string& AnimName,int iParentFrame, int iChildFrame,Vector2 & vRelativePos);

	IMAGE_TYPE GetImageType() const
	{
		return m_eImageType;
	}

	Anim2DFrame GetAnimFrame(int idx = 0)	const
	{
		return m_vecAnimFrame[idx];
	}

	const Vector2& GetPivot(int idx = 0) const
	{
		return m_vecAnimPivot[idx];
	}

	bool UseRelativePos() const
	{
		return m_bUseRelativePos;
	}

public:
	void AddSpriteComponent(class CSpriteComponent* pSprite)
	{
		m_SpriteList.push_back(pSprite);
	}
	void DeleteSpriteComponent(class CSpriteComponent* pSprite)
	{
		auto	iter = m_SpriteList.begin();
		auto	iterEnd = m_SpriteList.end();

		for (; iter != iterEnd; ++iter)
		{
			if (*iter == pSprite)
			{
				m_SpriteList.erase(iter);
				return;
			}
		}
	}

public:
	bool Init(class CTexture* pTexture, float fPlayTime = 1.f,
		float fPlayRate = 1.f);
	bool Init(const char* pFileName,
		const string& strPathName = RESOURCE_PATH);

	//호출해야할 Notify가 있으면 호출하는 Update함수
	void Update(int iFrame);

	//Frame정보 Setting
	//------------------------------------------------------
	void AddFrame(const Vector2& vStart, const Vector2& vEnd);
	void SetFrame(int iCount);
	//------------------------------------------------------

	//Notify추가 함수
	void AddNotify(const string& strName, int iFrame);

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

