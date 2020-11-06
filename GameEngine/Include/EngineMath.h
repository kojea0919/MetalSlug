#pragma once

#include <DirectXMath.h>
#include <DirectXPackedVector.h>

using namespace DirectX;

#include "Flag.h"

#define	PI	3.14159f

static float DegreeToRadian(float fAngle)
{
	return fAngle / 180.f * PI;
}

static float RadianToDegree(float fAngle)
{
	return fAngle * 180.f / PI;
}

/*
DirectX Math는 SIMD레지스터를 사용한다.
SIMD 레지스터는 128비트 연산을 한번에 처리해주는 방식으로 해주어서
16바이트 단위로 연산하기 때문에 연산속도가 엄청나게 빠르다.
vector4나 행렬 연산에 최적화 되어 있어서 이러한 연산방식을
사용한다.
*/
