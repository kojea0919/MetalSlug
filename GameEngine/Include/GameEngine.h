#pragma once

#define	DIRECTINPUT_VERSION	0x0800

#include "Types.h"

#define	ROOT_PATH		"RootPath"
#define	RESOURCE_PATH	"ResourcePath"
#define	TEXTURE_PATH	"TexturePath"
#define	DATA_PATH		"DataPath"
#define	SOUND_PATH		"SoundPath"
#define	SHADER_PATH		"ShaderPath"
#define ANI_PATH		"AniPath"

typedef struct _tagResolution
{
	int	iWidth;
	int	iHeight;
}Resolution, *PResolution;

typedef struct _tagVertex2D
{
	Vector3	vPos;
	Vector2 vUV;
	Vector4	vColor;
}Vertex2D, *PVertex2D;
