#include "Client.h"
#include "Input.h"
#include "CollisionManager.h"

bool GlobalSetting()
{

	GET_SINGLE(CInput)->AddAxisKey("MoveSide", 'D', 1.f);
	GET_SINGLE(CInput)->AddAxisKey("MoveSide", 'A', -1.f);

	GET_SINGLE(CInput)->AddAxisKey("RotationZ", 'E', 1.f);
	GET_SINGLE(CInput)->AddAxisKey("RotationZ", 'Q', -1.f);
	
	GET_SINGLE(CInput)->AddAxisKey("Scroll", (char)MOUSEWHEELUP, 1.f);
	GET_SINGLE(CInput)->AddAxisKey("Scroll", (char)MOUSEWHEELDOWN, -1.f);

	GET_SINGLE(CInput)->AddActionKey("Fire", 'J');
	GET_SINGLE(CInput)->AddActionKey("AimUp", 'W');
	GET_SINGLE(CInput)->AddActionKey("Down", 'S');
	GET_SINGLE(CInput)->AddActionKey("Bomb", 'L');

	//test
	GET_SINGLE(CInput)->AddActionKey("Test", 'T');

	//Collision Channel, Profile Setting
	//-----------------------------------
	GET_SINGLE(CCollisionManager)->CreateChannel("PlayerAttack",
		Collision_Type::Block);

	GET_SINGLE(CCollisionManager)->CreateProfile("PlauyerAttack",
		Collision_Channel::Custom1);
	
	GET_SINGLE(CCollisionManager)->SetProfileChannelState("Player",
		Collision_Channel::Custom1, Collision_Type::Ignore);
	GET_SINGLE(CCollisionManager)->SetProfileChannelState("PlayerAttack",
		Collision_Channel::Custom1, Collision_Type::Ignore);
	GET_SINGLE(CCollisionManager)->SetProfileChannelState("PlayerAttack",
		Collision_Channel::Player, Collision_Type::Ignore);
	//-----------------------------------

	return true;
}
