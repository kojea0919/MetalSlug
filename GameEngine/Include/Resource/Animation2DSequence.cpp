#include "Animation2DSequence.h"
#include "Texture.h"
#include "../Component/SpriteComponent.h"
#include "../PathManager.h"
#include "ResourceManager.h"

CAnimation2DSequence::CAnimation2DSequence()
	: m_pTexture(nullptr),
	m_fPlayRate(1.f),
	m_fPlayTime(0.f),
	m_fPlayTimeMax(1.f),
	m_iFrame(0),
	m_iFrameMax(1),
	m_bLoop(true),
	m_bUseRelativePos(false)
{
}

CAnimation2DSequence::~CAnimation2DSequence()
{
	SAFE_DELETE_MAP(m_mapChildAnimPos);
	SAFE_DELETE_VECLIST(m_NotifyList);
	SAFE_RELEASE(m_pTexture);
}

bool CAnimation2DSequence::GetTargetFramePosInfo(const string& AnimName, int iParentFrame, int iChildFrame, Vector2& vRelativePos)
{
	//해당 이름을 가진 자식 애니메이션이 없는 경우 pass
	//-----------------------------------------------
	auto iter = m_mapChildAnimPos.find(AnimName);
	if (iter == m_mapChildAnimPos.end())
		return false;
	//-----------------------------------------------

	Vector2 CurPos = (*(iter->second))[iParentFrame][iChildFrame];

	vRelativePos.x = CurPos.x;
	vRelativePos.y = CurPos.y;

	return true;
}

bool CAnimation2DSequence::Init(CTexture* pTexture, float fPlayTime, float fPlayRate)
{
	m_eImageType = pTexture->GetImageType();
	m_pTexture = pTexture;

	if (m_pTexture)
		m_pTexture->AddRef();

	m_fPlayRate = fPlayRate;
	m_fPlayTimeMax = fPlayTime;

    return true;
}

bool CAnimation2DSequence::Init(const char* pFileName, const string& strPathName)
{
	//파일 경로 Setting
	//--------------------------------
	char strFullPath[MAX_PATH] = {};

	const char * pPath = GET_SINGLE(CPathManager)->FindPathMultibyte(strPathName);
	if (pPath)
		strcpy_s(strFullPath, pPath);
	else
		return false;

	strcat_s(strFullPath, pFileName);
	//--------------------------------


	FILE* pFile = nullptr;
	fopen_s(&pFile, strFullPath, "rt");
	if (!pFile)
		return false;

	//저장 Format
	//-------------------

	//텍스처 경로
	//------------------------------------------
	char strTextureFullPath[MAX_PATH] = {};
	pPath = GET_SINGLE(CPathManager)->FindPathMultibyte(TEXTURE_PATH);
	if (pPath)
		strcpy_s(strTextureFullPath, pPath);
	else
		return false;

	char strTextureName[MAX_PATH] = {};
	fscanf_s(pFile, "%s\n", strTextureName,MAX_PATH);
		
	strcat_s(strTextureFullPath, strTextureName);

	//------------------------------------------

	//애니메이션이름
	//------------------------------------------
	char strAnimName[MAX_PATH] = {};
	fscanf_s(pFile, "%s\n", strAnimName,MAX_PATH);

	//해당 이름의 Sequence가 있으면 false return;
	CAnimation2DSequence * pTarget =  GET_SINGLE(CResourceManager)->FindAnim2DSequence(strAnimName);
	if (pTarget)
	{
		SAFE_RELEASE(pTarget);
		fclose(pFile);
		return false;
	}

	//Texture Loading
	//-------------------------------------------------
	TCHAR ConvertPath[MAX_PATH] = {};
	MultiByteToWideChar(CP_ACP, 0, strTextureFullPath, -1, ConvertPath, sizeof(TCHAR) * MAX_PATH);
	if (!GET_SINGLE(CResourceManager)->LoadTextureFullPath(strAnimName, ConvertPath))
	{
		fclose(pFile);
		return false;
	}

	m_pTexture = GET_SINGLE(CResourceManager)->FindTexture(strAnimName);
	if (!m_pTexture)
	{
		fclose(pFile);
		return false;
	}
	
	m_eImageType = m_pTexture->GetImageType();	
	//-------------------------------------------------

	//------------------------------------------

	//CutImage 수, Cut정보,	PIvot정보
	//-----------------------------------------
	int iCutImageCnt = 0;
	fscanf_s(pFile, "%d\n", &iCutImageCnt);

	m_iFrameMax = iCutImageCnt;

	for (int iCnt = 0; iCnt < iCutImageCnt; ++iCnt)
	{
		Anim2DFrame CurFrame;

		int iLeft = 0, iTop = 0, iRight = 0, iBottom = 0;
		fscanf_s(pFile, "%d %d %d %d\n", &iLeft, &iTop, &iRight, &iBottom);
		
		CurFrame.vStart = Vector2((float)iLeft, (float)iTop);
		CurFrame.vEnd = Vector2((float)iRight, (float)iBottom);

		m_vecAnimFrame.push_back(CurFrame);
	}
	for (int iCnt = 0; iCnt < iCutImageCnt; ++iCnt)
	{
		float fPivotX = 0, fPivotY = 0;
		fscanf_s(pFile, "%f %f\n", &fPivotX, &fPivotY);

		Vector2 CurPivot;
		CurPivot = Vector2((float)fPivotX, (float)fPivotY);

		m_vecAnimPivot.push_back(CurPivot);
	}
	//-----------------------------------------

	//재생속도
	fscanf_s(pFile, "%f\n", &m_fPlayRate);

	//재생시간
	fscanf_s(pFile, "%f\n", &m_fPlayTimeMax);

	//Loop 여부
	int iLoop = 0;
	fscanf_s(pFile, "%d\n", &iLoop);
	m_bLoop = iLoop;

	//-------------------

	m_strName = strAnimName;

	//Connect정보가 있으면 Setting
	//-----------------------------------------
	while (true)
	{
		//자식 Animation이름 Setting
		//----------------------------------------------
		char strChildAnimName[MAX_PATH] = {};
		fscanf_s(pFile, "%s\n", strChildAnimName, MAX_PATH);
		if (feof(pFile))
		{
			break;
		}
		//----------------------------------------------

		int iWidth = 0;
		int iHeight = 0;
		fscanf_s(pFile, "%d %d\n", &iWidth,&iHeight);

		m_bUseRelativePos = true;

		vector<vector<Vector2>>* pNewPosInfo = new vector<vector<Vector2>>;
		for (int iHeightCnt = 0; iHeightCnt < iHeight; ++iHeightCnt)
		{
			vector<Vector2> temp;
			for (int iWidthCnt = 0; iWidthCnt < iWidth; ++iWidthCnt)
			{
				int iRelativeX = 0, iRelativeY = 0;

				fscanf_s(pFile, "%d %d\n", &iRelativeX, &iRelativeY);

				temp.push_back(Vector2((float)iRelativeX, (float)iRelativeY));
			}
			pNewPosInfo->push_back(temp);
		}
		m_mapChildAnimPos.insert(make_pair(strChildAnimName, pNewPosInfo));
	}
	//-----------------------------------------


	fclose(pFile);

    return true;
}

void CAnimation2DSequence::Update(int iFrame)
{
	auto	iter = m_NotifyList.begin();
	auto	iterEnd = m_NotifyList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->m_iFrame == iFrame)
		{
			auto	iter1 = m_SpriteList.begin();
			auto	iter1End = m_SpriteList.end();

			for (; iter1 != iter1End; ++iter1)
			{
				(*iter1)->CallNotify((*iter)->m_strName);
			}
		}
	}
}

void CAnimation2DSequence::AddFrame(const Vector2& vStart, const Vector2& vEnd)
{
	Anim2DFrame tFrame;
	tFrame.vStart = vStart;
	tFrame.vEnd = vEnd;

	m_vecAnimFrame.push_back(tFrame);

	m_iFrameMax = (int)m_vecAnimFrame.size();
}

void CAnimation2DSequence::SetFrame(int iCount)
{
	//존재하는 정보 clear후 해당 Count만큼 resize
	m_vecAnimFrame.clear();
	m_vecAnimFrame.resize(iCount);

	m_iFrameMax = iCount;
}

void CAnimation2DSequence::AddNotify(const string& strName, int iFrame)
{
	CAnimation2DNotify* pNotify = new CAnimation2DNotify;

	pNotify->Create(strName, iFrame);
	pNotify->m_pOwner = this;

	m_NotifyList.push_back(pNotify);
}

void CAnimation2DSequence::Save(FILE* pFile)
{

}

void CAnimation2DSequence::Load(FILE* pFile)
{
}
