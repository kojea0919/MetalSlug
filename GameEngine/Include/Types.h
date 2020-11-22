#pragma once

#include <Windows.h>
#include <list>
#include <vector>
#include <unordered_map>
#include <crtdbg.h>
#include <tchar.h>
#include <functional>
#include <string>
#include <math.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <typeinfo>
#include <dinput.h>
#include <algorithm>
#include <dwrite.h>
#include <d2d1.h.>
#include "Resource/DirectXTex.h"

#include "Macro.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix.h"
#include "fmod.hpp"

using namespace std;
using namespace FMOD;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d2d1.lib")

#ifdef _WIN64

#pragma comment(lib, "fmod64_vc.lib")

#ifdef _DEBUG
#pragma comment(lib,"DirectXTex_Debug.lib")
#else
#pragma comment(lib,"DirectXTex.lib")
#endif

#else

#pragma comment(lib,"fmod_vc.lib")

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex32_Debug.lib")
#else
#pragma comment(lib, "DirectXTex32.lib")
#endif

#endif

//정점 버퍼
//------------------------------
typedef struct _tagVertexBuffer
{
	ID3D11Buffer* pBuffer;	//버퍼
	int				iSize;		//정점 1개의 크기
	int				iCount;		//정점 수
	D3D11_USAGE		eUsage;

	void* pData;

	_tagVertexBuffer()
	{
		pBuffer = nullptr;
		pData = nullptr;
		iSize = 0;
		iCount = 0;
	}
}VertexBuffer, * PVertexBuffer;
//------------------------------


//인덱스 버퍼
//------------------------------
typedef struct _tagIndexBuffer
{
	ID3D11Buffer* pBuffer;	//버퍼
	int				iSize;		//정점 1개의 크기
	int				iCount;		//정점 수
	D3D11_USAGE		eUsage;
	DXGI_FORMAT		eFmt;
}IndexBuffer, * PIndexBuffer;
//------------------------------

typedef struct _tagConstantBuffer
{
	ID3D11Buffer* pBuffer;
	int			iSize;
	int			iRegister;
	int			iShaderType;
}ConstantBuffer, *PConstantBuffer;


typedef struct _tagMaterialCBuffer
{
	Vector4		vDif;
	Vector4		vAmb;
	Vector4		vSpc;

	//Texture를 사용하는지 나타내는 플래그
	//------------------------------------
	int			iDifTex;
	int			iNrmTex;
	int			iSpcTex;
	int			iEmpty;
	//------------------------------------
}MaterialCBuffer, *PMaterialCBuffer;

typedef struct _tagTransformCBuffer
{
	Matrix		matWorld;
	Matrix		matView;
	Matrix		matProj;
	Matrix		matWV;
	Matrix		matWVP;
	Vector3		vPivot;
	float		fEmpty1;
	Vector3		vMeshSize;
	float		fEmpty2;
}TransformCBuffer, *PTransformCBuffer;

typedef struct _tagHierarchyName
{
	string	strName;
	string	strParent;
}HierarchyName, *PHierarchyName;

typedef struct _tagSpriteCBuffer
{
	int			iImageType;
	int			iAnimType;
	Vector2		vFrameStart;
	Vector2		vFrameEnd;
	Vector2		vImageSize;
}SpriteCBuffer, *PSpriteCBuffer;

typedef struct _tagBarCBuffer
{
	int			iBarDir;
	float		fPercent;
	Vector2		vEmpty;
}BarCBuffer, * PBarCBuffer;

typedef struct _tagButtonCBuffer
{
	int			iCheck;
	Vector3		vEmpty;
}ButtonCBuffer, *PButtonCBuffer;

typedef struct _tagRectInfo
{
	float	fL;
	float	fT;
	float	fR;
	float	fB;

	_tagRectInfo() :
		fL(0.f),
		fT(0.f),
		fR(0.f),
		fB(0.f)
	{
	}
}RectInfo, * PRectInfo;

typedef struct _tagSphereInfo
{
	Vector3		vCenter;
	float		fRadius;

	_tagSphereInfo()
	{
		fRadius = 0.f;
	}
}SphereInfo, * PSphereInfo;

typedef struct _tagOBBInfo
{
	Vector3		vCenter;
	Vector3		vAxis[AXIS::AXIS_END];

	Vector3		vLength;

	_tagOBBInfo()
	{
		vAxis[AXIS::AXIS_X] = Vector3(1, 0, 0);
		vAxis[AXIS::AXIS_Y] = Vector3(0, 1, 0);
		vAxis[AXIS::AXIS_Z] = Vector3(0, 0, 1);
	}
}OBBInfo,*POBBInfo;

enum class PIXEL_COLLISION_TYPE
{
	Color_Ignore,
	Less_Alpha_Ignore,
	Color_Confirm
};

typedef struct _tagPixelColliderInfo
{
	unsigned char* pPixel;
	int				iWidth;
	int				iHeight;
	PIXEL_COLLISION_TYPE	eType;
	unsigned char	IgnoreColor[4];
	unsigned char	CheckColor[4];
	RectInfo		tRC;

	_tagPixelColliderInfo()
	{
		pPixel = nullptr;
		eType = PIXEL_COLLISION_TYPE::Less_Alpha_Ignore;
		memset(IgnoreColor, 0, sizeof(unsigned char) * 4);
		memset(CheckColor, 0, sizeof(unsigned char) * 4);

		iWidth = 0;
		iHeight = 0;
	}

	~_tagPixelColliderInfo()
	{
		SAFE_DELETE_ARRAY(pPixel);
	}
}PixelColliderInfo, * PPixelColliderInfo;

typedef struct _tagTimeCBuffer
{
	Vector4		m_vDeltaTime;
}DeltaTimeCBuffer,*PDeltaTimeCBuffer;

typedef struct _tagCollisionChannel
{
	string				strName;
	Collision_Channel	eChannel;
	Collision_Type		eType;
}CollisionChannel, * PCollisionChannel;

typedef struct _tagCollisionProfile
{
	string		strName;
	Collision_Channel	eChannel;
	vector<CollisionChannel>	vecChannel;
}CollisionProfile, * PCollisionProfile;

//Texture정보 구조체
//------------------------------------------
typedef struct _tagMaterialTextureInfo
{
	TEXTURE_LINK	eTextureLink;	//텍스처 종류
	class CTexture* pTexture;		//사용할 텍스처
	int				iRegister;		//사용할 레지스터
	int				iShaderType;	//적용할 셰이더 타입
}MaterialTextureInfo, * PMaterialTextureInfo;
//------------------------------------------

//2D Instancing용 데이터 구조체
//----------------------------------
typedef struct _tagInstancingData2D
{
	Matrix	matWVP;
	Vector3	vMeshSize;
	Vector3	vMeshPivot;
	Vector2 vFrameStart;
	Vector2 vFrameEnd;
	Vector2	vImageSize;
}InstancingData2D, * PInstancingData2D;
//----------------------------------

//3D Instancing용 데이터 구조체
//----------------------------------
typedef struct _tagInstancingData
{
	Matrix	matWVP;
	Vector3	vMeshSize;
	Vector3	vMeshPivot;
}InstancingData, * PInstancingData;
//----------------------------

//Collider Instancing용 데이터 구조체
//----------------------------------
typedef struct _tagInstancingDataCollider
{
	Matrix	matWVP;
	Vector3	vMeshSize;
	Vector3	vMeshPivot;
	bool bCollision;
}InstancingDataCollider, * PInstancingDataCollider;
//----------------------------------------