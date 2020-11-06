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
	//�ش� �̸��� ���� �ڽ� �ִϸ��̼��� ���� ��� pass
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
	//���� ��� Setting
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

	//���� Format
	//-------------------

	//�ؽ�ó ���
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

	//�ִϸ��̼��̸�
	//------------------------------------------
	char strAnimName[MAX_PATH] = {};
	fscanf_s(pFile, "%s\n", strAnimName,MAX_PATH);

	//�ش� �̸��� Sequence�� ������ false return;
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

	//CutImage ��, Cut����,	PIvot����
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

	//����ӵ�
	fscanf_s(pFile, "%f\n", &m_fPlayRate);

	//����ð�
	fscanf_s(pFile, "%f\n", &m_fPlayTimeMax);

	//Loop ����
	int iLoop = 0;
	fscanf_s(pFile, "%d\n", &iLoop);
	m_bLoop = iLoop;

	//-------------------

	m_strName = strAnimName;

	//Connect������ ������ Setting
	//-----------------------------------------
	while (true)
	{
		//�ڽ� Animation�̸� Setting
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
	//�����ϴ� ���� clear�� �ش� Count��ŭ resize
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
