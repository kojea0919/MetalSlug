#pragma once
#include "../Ref.h"
#include "Animation2DNotify.h"

//Anim2DFrame����ü
//Atlas�̹��� ���� �� �ִϸ��̼��� ���� ��ǥ�� �� ��ǥ ����
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

	//�̹��� cut ����
	vector<Anim2DFrame>	m_vecAnimFrame;

	//�̹��� pivot����
	vector<Vector2> m_vecAnimPivot;

	//�̹��� ��� ��ġ ����
	//-------------------------------------
	bool			m_bUseRelativePos;

	//�ش� SpriteComponent�� �ڽ����� �����ϴ� SpriteComponent����
	//RelativePos�� �����ϴ� Map
	unordered_map<string, vector<vector<Vector2>>*> m_mapChildAnimPos;
	//-------------------------------------

	//�ִϸ��̼��� ������ �ִ� SpriteComponentList
	list<class CSpriteComponent*>	m_SpriteList;

	//�ִϸ��̼ǿ� ��ϵ� NotifyList
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

	//ȣ���ؾ��� Notify�� ������ ȣ���ϴ� Update�Լ�
	void Update(int iFrame);

	//Frame���� Setting
	//------------------------------------------------------
	void AddFrame(const Vector2& vStart, const Vector2& vEnd);
	void SetFrame(int iCount);
	//------------------------------------------------------

	//Notify�߰� �Լ�
	void AddNotify(const string& strName, int iFrame);

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

