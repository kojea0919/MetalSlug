#include "CollisionManager.h"

DEFINITION_SINGLE(CCollisionManager)

CCollisionManager::CCollisionManager()
	: m_bColliderRender(true)
{

}

CCollisionManager::~CCollisionManager()
{
	SAFE_DELETE_MAP(m_mapProfile);
	SAFE_DELETE_VECLIST(m_vecChannel);
}

bool CCollisionManager::Init()
{
	CreateChannel("Static", Collision_Type::Block);
	CreateChannel("Player", Collision_Type::Block);
	CreateChannel("Monster", Collision_Type::Block);
	CreateChannel("NPC", Collision_Type::Block);

	CreateProfile("Static", Collision_Channel::Static);
	CreateProfile("Player", Collision_Channel::Player);
	CreateProfile("Monster", Collision_Channel::Monster);
	CreateProfile("NPC", Collision_Channel::NPC);

	//�÷��̾�鳢���� ignore
	SetProfileChannelState("Player", Collision_Channel::Player,
		Collision_Type::Ignore);

	return true;
}

bool CCollisionManager::CreateChannel(const string& strName, Collision_Type eDefaultType)
{
	//��� Channel�� ������� ���
	//----------------------------------------
	if (m_vecChannel.size() == (size_t)Collision_Channel::End)
		return false;
	//----------------------------------------

	//���ο� Channel ����
	//----------------------------------------
	PCollisionChannel	pChannel = new CollisionChannel;
	pChannel->strName = strName;
	pChannel->eChannel = (Collision_Channel)m_vecChannel.size();
	pChannel->eType = eDefaultType;
	//----------------------------------------

	m_vecChannel.push_back(pChannel);

	//�߰��� ä���� �� �������Ͽ� �߰�
	//----------------------------------------
	auto iter = m_mapProfile.begin();
	auto iterEnd = m_mapProfile.end();

	for (; iter != iterEnd; ++iter)
	{
		iter->second->vecChannel.push_back(*pChannel);
	}
	//----------------------------------------

	return true;
}

bool CCollisionManager::CreateProfile(const string& strName, Collision_Channel eChannel)
{
	PCollisionProfile pProfile = FindProfile(strName);

	if (pProfile)
		return false;

	pProfile = new CollisionProfile;

	pProfile->strName = strName;
	pProfile->eChannel = eChannel;

	m_mapProfile.insert(make_pair(strName, pProfile));

	//���� ������ ä�� ���� �߰�
	//------------------------------------
	size_t iSize = m_vecChannel.size();

	for (size_t iCnt = 0; iCnt < iSize; ++iCnt)
	{
		pProfile->vecChannel.push_back(*m_vecChannel[iCnt]);
	}
	//------------------------------------

	return true;
}

bool CCollisionManager::SetProfileChannelState(const string& strName, Collision_Channel eChannel, Collision_Type eType)
{
	PCollisionProfile	pProfile = FindProfile(strName);

	if (!pProfile)
		return false;

	pProfile->vecChannel[(int)eChannel].eType = eType;

	return true;
}

PCollisionProfile CCollisionManager::FindProfile(const string& strName)
{
	auto iter = m_mapProfile.find(strName);

	if (iter == m_mapProfile.end())
		return nullptr;

	return iter->second;
}
