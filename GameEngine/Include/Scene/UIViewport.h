#pragma once

#include "../GameEngine.h"

class CUIViewport
{
	friend class CScene;

protected:
	CUIViewport();
	~CUIViewport();

protected:
	class CScene* m_pScene;
	list<class CUIObject*> m_UIList;

public:
	const list<class CUIObject*>& GetUIList() const
	{
		return m_UIList;
	}

public:
	void AddUI(class CUIObject* pUI);

public:
	class CScene* GetScene() const
	{
		return m_pScene;
	}

public:
	bool Init();
	void Start();
	void Update(float fTime);
	void PostUpdate(float fTime);
	void PrevRender(float fTime);
	void Render(float fTime);
	void PostRender(float fTime);

public:
	void Save(FILE* pFile);
	void Load(FILE* pFile);

private:
	static bool SortUI(class CUIObject* pSrc, class CUIObject* pDest);
};

