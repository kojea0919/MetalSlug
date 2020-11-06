
#include "Vector4.h"

_tagVector4	_tagVector4::Black(0.f, 0.f, 0.f, 1.f);
_tagVector4	_tagVector4::White(1.f, 1.f, 1.f, 1.f);
_tagVector4	_tagVector4::Red(1.f, 0.f, 0.f, 1.f);
_tagVector4	_tagVector4::Green(0.f, 1.f, 0.f, 1.f);
_tagVector4	_tagVector4::Blue(0.f, 0.f, 1.f, 1.f);

_tagVector4::_tagVector4() :
	x(0.f),
	y(0.f),
	z(0.f)
{
}

_tagVector4::_tagVector4(float _x, float _y, float _z, float _w) :
	x(_x),
	y(_y),
	z(_z),
	w(_w)
{
}

_tagVector4::_tagVector4(const _tagVector4& v)
{
	*this = v;
}

_tagVector4::_tagVector4(const XMVECTOR& v)
{
	XMStoreFloat4((XMFLOAT4*)this, v);
}

_tagVector4& _tagVector4::operator=(const _tagVector4& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;

	return *this;
}

_tagVector4& _tagVector4::operator=(const XMVECTOR& v)
{
	XMStoreFloat4((XMFLOAT4*)this, v);

	return *this;
}

_tagVector4& _tagVector4::operator=(float f)
{
	x = f;
	y = f;
	z = f;
	w = f;

	return *this;
}

bool _tagVector4::operator==(const _tagVector4& v) const
{
	return (x == v.x && y == v.y && z == v.z && w == v.w);
}

bool _tagVector4::operator==(const XMVECTOR& v) const
{
	_tagVector4	_v(v);

	return (x == _v.x && y == _v.y && z == _v.z && w == _v.w);
}

bool _tagVector4::operator!=(const _tagVector4& v) const
{
	return (x != v.x || y != v.y || z != v.z || w != v.w);
}

bool _tagVector4::operator!=(const XMVECTOR& v) const
{
	_tagVector4	_v(v);

	return (x != _v.x || y != _v.y || z != _v.z || w != _v.w);
}

float _tagVector4::operator[](const int& idx) const
{
	switch (idx)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	case 3:
		return w;
	}

	assert(false);
	return -1.f;
}

float& _tagVector4::operator[](const int& idx)
{
	switch (idx)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	case 3:
		return w;
	}

	assert(false);

	return w;
}

// +
_tagVector4 _tagVector4::operator+(const _tagVector4& v) const
{
	XMVECTOR	v1 = Convert();
	XMVECTOR	v2 = v.Convert();

	return _tagVector4(v1 + v2);
}

_tagVector4 _tagVector4::operator+(const XMVECTOR& v) const
{
	XMVECTOR	v1 = Convert();

	return _tagVector4(v1 + v);
}

_tagVector4 _tagVector4::operator+(float f) const
{
	return _tagVector4(x + f, y + f, z + f, w + f);
}

_tagVector4 _tagVector4::operator+=(const _tagVector4& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;

	return *this;
}

_tagVector4 _tagVector4::operator+=(const XMVECTOR& v)
{
	XMVECTOR	v1 = Convert();
	v1 += v;

	Convert(v1);

	return *this;
}

_tagVector4 _tagVector4::operator+=(float f)
{
	x += f;
	y += f;
	z += f;
	w += f;
	return *this;
}

// -
_tagVector4 _tagVector4::operator-(const _tagVector4& v) const
{
	XMVECTOR	v1 = Convert();
	XMVECTOR	v2 = v.Convert();

	return _tagVector4(v1 - v2);
}

_tagVector4 _tagVector4::operator-(const XMVECTOR& v) const
{
	XMVECTOR	v1 = Convert();

	return _tagVector4(v1 - v);
}

_tagVector4 _tagVector4::operator-(float f) const
{
	return _tagVector4(x - f, y - f, z - f, w - f);
}

_tagVector4 _tagVector4::operator-=(const _tagVector4& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;

	return *this;
}

_tagVector4 _tagVector4::operator-=(const XMVECTOR& v)
{
	XMVECTOR	v1 = Convert();
	v1 -= v;

	Convert(v1);

	return *this;
}

_tagVector4 _tagVector4::operator-=(float f)
{
	x -= f;
	y -= f;
	z -= f;
	w -= f;
	return *this;
}

// *
_tagVector4 _tagVector4::operator*(const _tagVector4& v) const
{
	XMVECTOR	v1 = Convert();
	XMVECTOR	v2 = v.Convert();

	return _tagVector4(v1 * v2);
}

_tagVector4 _tagVector4::operator*(const XMVECTOR& v) const
{
	XMVECTOR	v1 = Convert();

	return _tagVector4(v1 * v);
}

_tagVector4 _tagVector4::operator*(float f) const
{
	return _tagVector4(x * f, y * f, z * f, w * f);
}

_tagVector4 _tagVector4::operator*=(const _tagVector4& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;

	return *this;
}

_tagVector4 _tagVector4::operator*=(const XMVECTOR& v)
{
	XMVECTOR	v1 = Convert();
	v1 *= v;

	Convert(v1);

	return *this;
}

_tagVector4 _tagVector4::operator*=(float f)
{
	x *= f;
	y *= f;
	z *= f;
	w *= f;
	return *this;
}

// /
_tagVector4 _tagVector4::operator/(const _tagVector4& v) const
{
	XMVECTOR	v1 = Convert();
	XMVECTOR	v2 = v.Convert();

	return _tagVector4(v1 / v2);
}

_tagVector4 _tagVector4::operator/(const XMVECTOR& v) const
{
	XMVECTOR	v1 = Convert();

	return _tagVector4(v1 / v);
}

_tagVector4 _tagVector4::operator/(float f) const
{
	return _tagVector4(x / f, y / f, z / f, w / f);
}

_tagVector4 _tagVector4::operator/=(const _tagVector4& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;

	return *this;
}

_tagVector4 _tagVector4::operator/=(const XMVECTOR& v)
{
	XMVECTOR	v1 = Convert();
	v1 /= v;

	Convert(v1);

	return *this;
}

_tagVector4 _tagVector4::operator/=(float f)
{
	x /= f;
	y /= f;
	z /= f;
	w /= f;
	return *this;
}

XMVECTOR _tagVector4::Convert() const
{
	return XMLoadFloat4((XMFLOAT4*)this);
}

void _tagVector4::Convert(const XMVECTOR& v)
{
	XMStoreFloat4((XMFLOAT4*)this, v);
}
