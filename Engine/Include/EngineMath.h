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
DirectX Math�� SIMD�������͸� ����Ѵ�.
SIMD �������ʹ� 128��Ʈ ������ �ѹ��� ó�����ִ� ������� ���־
16����Ʈ ������ �����ϱ� ������ ����ӵ��� ��û���� ������.
vector4�� ��� ���꿡 ����ȭ �Ǿ� �־ �̷��� ��������
����Ѵ�.
*/
