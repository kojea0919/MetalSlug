#pragma once

enum AXIS
{
	AXIS_X,
	AXIS_Y,
	AXIS_Z,
	AXIS_END
};

enum class SHADER_TYPE
{
	GRAPHIC,
	COMPUTE
};

enum class COMPONENT_TYPE
{
	SCENE,
	OBJECT
};

enum class CBUFFER_SHADER_TYPE
{
	CBUFFER_VERTEX = 0x1,
	CBUFFER_PIXEL = 0x2,
	CBUFFER_DOMAIN = 0x4,
	CBUFFER_HULL = 0x8,
	CBUFFER_GEOMETRY = 0x10,
	CBUFFER_COMPUTE = 0x20
};

enum class SCENECOMPONENT_TYPE
{
	ST_3D,
	ST_2D,
	ST_UI
};

enum class SCENECOMPONENT_CLASS_TYPE
{
	SCENE,
	MESH2D,
	STATICMESH,
	CAMERA,
	SPRITE,
	COLLIDERRECT,
	COLLIDERSPHERE2D,
	COLLIDEROBB2D,
	COLLIDERPIXEL,
	TILEMAP
};

enum class CAMERA_TYPE
{
	CAM3D,
	CAM2D,
	CAMUI
};

enum class IMAGE_TYPE
{
	ATLAS,
	ARRAY
};

enum class TEXTURE_LINK
{
	DIFFUSE,
	NORMAL,
	SPECULAR,
	CUSTOM
};

enum class COLLIDER_TYPE
{
	RECT,
	SPHERE2D,
	OBB2D,
	PIXEL,
	LINE
};

enum class MESH_TYPE
{
	MESH2D,
	STATICMESH,
	ANIMATIONMESH
};

enum class Collision_Channel
{
	Static,
	Player,
	Monster,
	NPC,
	Custom1,
	Custom2,
	Custom3,
	Custom4,
	Custom5,
	Custom6,
	Custom7,
	Custom8,
	Custom9,
	Custom10,
	Custom11,
	Custom12,
	Custom13,
	Custom14,
	Custom15,
	Custom16,
	Custom17,
	Custom18,
	Custom19,
	Custom20,
	End
};

enum class Collision_Type
{
	Ignore,
	Block
};

enum class Collision_State
{
	Begin,
	End,
	Max
};

enum class GameMode_Type
{
	GMT_MAIN,
	GMT_LOBBY,
	End
};

enum class Render_Priority
{
	RP_OTHER,
	RP_LOW,
	RP_MID,
	RP_HIGH
};

enum class Button_State
{
	Normal,
	MouseOn,
	Click,
	Disable,
	End
};

enum class TILE_SHAPE
{
	Rect,
	Rhombus
};

enum class Material_Type
{
	Origin,
	Instance
};