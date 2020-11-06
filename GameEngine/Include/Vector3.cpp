#include "Vector3.h"


_tagVector3	_tagVector3::Zero(0.f, 0.f, 0.f);
_tagVector3	_tagVector3::One(1.f, 1.f, 1.f);
_tagVector3	_tagVector3::Axis[AXIS_END] =
{
	_tagVector3(1.f, 0.f, 0.f),
	_tagVector3(0.f, 1.f, 0.f),
	_tagVector3(0.f, 0.f, 1.f)
};

_tagVector3::_tagVector3() :
	x(0.f),
	y(0.f),
	z(0.f)
{
}

_tagVector3::_tagVector3(float _x, float _y, float _z) :
	x(_x),
	y(_y),
	z(_z)
{
}

_tagVector3::_tagVector3(const _tagVector3& v)
{
	*this = v;
}

_tagVector3::_tagVector3(const XMVECTOR& v)
{
	XMStoreFloat3((XMFLOAT3*)this, v);
}

_tagVector3& _tagVector3::operator=(const _tagVector3& v)
{
	x = v.x;
	y = v.y;
	z = v.z;

	return *this;
}

_tagVector3& _tagVector3::operator=(const XMVECTOR& v)
{
	XMStoreFloat3((XMFLOAT3*)this, v);

	return *this;
}

_tagVector3& _tagVector3::operator=(float f)
{
	x = f;
	y = f;
	z = f;

	return *this;
}

bool _tagVector3::operator==(const _tagVector3& v) const
{
	return (x == v.x && y == v.y && z == v.z);
}

bool _tagVector3::operator==(const XMVECTOR& v) const
{
	_tagVector3	_v(v);

	return (x == _v.x && y == _v.y && z == _v.z);
}

bool _tagVector3::operator!=(const _tagVector3& v) const
{
	return (x != v.x || y != v.y || z != v.z);
}

bool _tagVector3::operator!=(const XMVECTOR& v) const
{
	_tagVector3	_v(v);

	return (x != _v.x || y != _v.y || z != _v.z);
}

float _tagVector3::operator[](const int& idx) const
{
	switch (idx)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	}

	assert(false);
	return -1.f;
}

float& _tagVector3::operator[](const int& idx)
{
	switch (idx)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	}

	assert(false);
	return x;
}

// +
_tagVector3 _tagVector3::operator+(const _tagVector3& v) const
{
	XMVECTOR	v1 = Convert();
	XMVECTOR	v2 = v.Convert();

	return _tagVector3(v1 + v2);
}

_tagVector3 _tagVector3::operator+(const XMVECTOR& v) const
{
	XMVECTOR	v1 = Convert();

	return _tagVector3(v1 + v);
}

_tagVector3 _tagVector3::operator+(float f) const
{
	return _tagVector3(x + f, y + f, z + f);
}

_tagVector3 _tagVector3::operator+=(const _tagVector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;

	return *this;
}

_tagVector3 _tagVector3::operator+=(const XMVECTOR& v)
{
	XMVECTOR	v1 = Convert();
	v1 += v;

	Convert(v1);

	return *this;
}

_tagVector3 _tagVector3::operator+=(float f)
{
	x += f;
	y += f;
	z += f;
	return *this;
}

// -
_tagVector3 _tagVector3::operator-(const _tagVector3& v) const
{
	XMVECTOR	v1 = Convert();
	XMVECTOR	v2 = v.Convert();

	return _tagVector3(v1 - v2);
}

_tagVector3 _tagVector3::operator-(const XMVECTOR& v) const
{
	XMVECTOR	v1 = Convert();

	return _tagVector3(v1 - v);
}

_tagVector3 _tagVector3::operator-(float f) const
{
	return _tagVector3(x - f, y - f, z - f);
}

_tagVector3 _tagVector3::operator-=(const _tagVector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;

	return *this;
}

_tagVector3 _tagVector3::operator-=(const XMVECTOR& v)
{
	XMVECTOR	v1 = Convert();
	v1 -= v;

	Convert(v1);

	return *this;
}

_tagVector3 _tagVector3::operator-=(float f)
{
	x -= f;
	y -= f;
	z -= f;
	return *this;
}

// *
_tagVector3 _tagVector3::operator*(const _tagVector3& v) const
{
	XMVECTOR	v1 = Convert();
	XMVECTOR	v2 = v.Convert();

	return _tagVector3(v1 * v2);
}

_tagVector3 _tagVector3::operator*(const XMVECTOR& v) const
{
	XMVECTOR	v1 = Convert();

	return _tagVector3(v1 * v);
}

_tagVector3 _tagVector3::operator*(float f) const
{
	return _tagVector3(x * f, y * f, z * f);
}

_tagVector3 _tagVector3::operator*=(const _tagVector3& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;

	return *this;
}

_tagVector3 _tagVector3::operator*=(const XMVECTOR& v)
{
	XMVECTOR	v1 = Convert();
	v1 *= v;

	Convert(v1);

	return *this;
}

_tagVector3 _tagVector3::operator*=(float f)
{
	x *= f;
	y *= f;
	z *= f;
	return *this;
}

// /
_tagVector3 _tagVector3::operator/(const _tagVector3& v) const
{
	XMVECTOR	v1 = Convert();
	XMVECTOR	v2 = v.Convert();

	return _tagVector3(v1 / v2);
}

_tagVector3 _tagVector3::operator/(const XMVECTOR& v) const
{
	XMVECTOR	v1 = Convert();

	return _tagVector3(v1 / v);
}

_tagVector3 _tagVector3::operator/(float f) const
{
	return _tagVector3(x / f, y / f, z / f);
}

_tagVector3 _tagVector3::operator/=(const _tagVector3& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;

	return *this;
}

_tagVector3 _tagVector3::operator/=(const XMVECTOR& v)
{
	XMVECTOR	v1 = Convert();
	v1 /= v;

	Convert(v1);

	return *this;
}

_tagVector3 _tagVector3::operator/=(float f)
{
	x /= f;
	y /= f;
	z /= f;
	return *this;
}

float _tagVector3::Length() const
{
	XMVECTOR	v = Convert();


	// XMVECTOR는 x, y, z, w 모두 갖고 있는 16바이트 타입이다.
	// 크기를 구하면 x, y, z, w에 모두 크기를 넣어주어서
	// x값을 리턴해주면 된다.
	return XMVectorGetX(XMVector3Length(v));
}

void _tagVector3::Normalize()
{
	XMVECTOR	v = Convert();
	Convert(XMVector3Normalize(v));
}

float _tagVector3::Distance(const _tagVector3& v) const
{
	_tagVector3	vResult = *this - v;

	return vResult.Length();
}

float _tagVector3::Dot(const _tagVector3& v) const
{
	XMVECTOR	v1 = Convert();
	XMVECTOR	v2 = v.Convert();

	return XMVectorGetX(XMVector3Dot(v1, v2));
}

float _tagVector3::Angle(const _tagVector3& v) const
{
	_tagVector3	v1 = *this;
	_tagVector3	v2 = v;

	v1.Normalize();
	v2.Normalize();

	float	fAngle = v1.Dot(v2);

	fAngle = acosf(fAngle) * 180.f / PI;

	return fAngle;
}

_tagVector3 _tagVector3::Cross(const _tagVector3& v) const
{
	XMVECTOR	v1 = Convert();
	XMVECTOR	v2 = v.Convert();

	return _tagVector3(XMVector3Cross(v1, v2));
}

// 위치 적용 안하고 곱할때
_tagVector3 _tagVector3::TransformNormal(const Matrix& _m)	const
{
	return _tagVector3(XMVector3TransformNormal(Convert(), _m.m));
}

// 위치 적용 하고 곱할때
_tagVector3 _tagVector3::TransformCoord(const Matrix& _m)	const
{
	return _tagVector3(XMVector3TransformCoord(Convert(), _m.m));
}

XMVECTOR _tagVector3::Convert() const
{
	return XMLoadFloat3((XMFLOAT3*)this);
}

void _tagVector3::Convert(const XMVECTOR& v)
{
	XMStoreFloat3((XMFLOAT3*)this, v);
}

float _tagVector3::Length(const _tagVector3& v)
{
	XMVECTOR	v1 = v.Convert();

	return XMVectorGetX(XMVector3Length(v1));
}

_tagVector3 _tagVector3::Normalize(const _tagVector3& v)
{
	XMVECTOR	v1 = v.Convert();
	return _tagVector3(XMVector3Normalize(v1));
}

float _tagVector3::Distance(const _tagVector3& vSrc, const _tagVector3& vDest)
{
	_tagVector3	vResult = vSrc - vDest;

	return vResult.Length();
}
