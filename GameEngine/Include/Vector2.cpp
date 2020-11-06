
#include "Vector2.h"

_tagVector2::_tagVector2() :
	x(0.f),
	y(0.f)
{
}

_tagVector2::_tagVector2(float _x, float _y) :
	x(_x),
	y(_y)
{
}

_tagVector2::_tagVector2(const _tagVector2& v)
{
	*this = v;
}

_tagVector2::_tagVector2(const XMVECTOR& v)
{
	XMStoreFloat2((XMFLOAT2*)this, v);
}

_tagVector2& _tagVector2::operator=(const _tagVector2& v)
{
	x = v.x;
	y = v.y;

	return *this;
}

_tagVector2& _tagVector2::operator=(const XMVECTOR& v)
{
	XMStoreFloat2((XMFLOAT2*)this, v);

	return *this;
}

_tagVector2& _tagVector2::operator=(float f)
{
	x = f;
	y = f;

	return *this;
}

bool _tagVector2::operator==(const _tagVector2& v) const
{
	return (x == v.x && y == v.y);
}

bool _tagVector2::operator==(const XMVECTOR& v) const
{
	_tagVector2	_v(v);

	return (x == _v.x && y == _v.y);
}

bool _tagVector2::operator!=(const _tagVector2& v) const
{
	return (x != v.x || y != v.y);
}

bool _tagVector2::operator!=(const XMVECTOR& v) const
{
	_tagVector2	_v(v);

	return (x != _v.x || y != _v.y);
}

float _tagVector2::operator[](const int& idx) const
{
	switch (idx)
	{
	case 0:
		return x;
	case 1:
		return y;
	}

	assert(false);
	return -1.f;
}

float& _tagVector2::operator[](const int& idx)
{
	switch (idx)
	{
	case 0:
		return x;
	case 1:
		return y;
	}

	assert(false);
}

// +
_tagVector2 _tagVector2::operator+(const _tagVector2& v) const
{
	XMVECTOR	v1 = Convert();
	XMVECTOR	v2 = v.Convert();

	return _tagVector2(v1 + v2);
}

_tagVector2 _tagVector2::operator+(const XMVECTOR& v) const
{
	XMVECTOR	v1 = Convert();

	return _tagVector2(v1 + v);
}

_tagVector2 _tagVector2::operator+(float f) const
{
	return _tagVector2(x + f, y + f);
}

_tagVector2 _tagVector2::operator+=(const _tagVector2& v)
{
	x += v.x;
	y += v.y;

	return *this;
}

_tagVector2 _tagVector2::operator+=(const XMVECTOR& v)
{
	XMVECTOR	v1 = Convert();
	v1 += v;

	Convert(v1);

	return *this;
}

_tagVector2 _tagVector2::operator+=(float f)
{
	x += f;
	y += f;
	return *this;
}

// -
_tagVector2 _tagVector2::operator-(const _tagVector2& v) const
{
	XMVECTOR	v1 = Convert();
	XMVECTOR	v2 = v.Convert();

	return _tagVector2(v1 - v2);
}

_tagVector2 _tagVector2::operator-(const XMVECTOR& v) const
{
	XMVECTOR	v1 = Convert();

	return _tagVector2(v1 - v);
}

_tagVector2 _tagVector2::operator-(float f) const
{
	return _tagVector2(x - f, y - f);
}

_tagVector2 _tagVector2::operator-=(const _tagVector2& v)
{
	x -= v.x;
	y -= v.y;

	return *this;
}

_tagVector2 _tagVector2::operator-=(const XMVECTOR& v)
{
	XMVECTOR	v1 = Convert();
	v1 -= v;

	Convert(v1);

	return *this;
}

_tagVector2 _tagVector2::operator-=(float f)
{
	x -= f;
	y -= f;
	return *this;
}

// *
_tagVector2 _tagVector2::operator*(const _tagVector2& v) const
{
	XMVECTOR	v1 = Convert();
	XMVECTOR	v2 = v.Convert();

	return _tagVector2(v1 * v2);
}

_tagVector2 _tagVector2::operator*(const XMVECTOR& v) const
{
	XMVECTOR	v1 = Convert();

	return _tagVector2(v1 * v);
}

_tagVector2 _tagVector2::operator*(float f) const
{
	return _tagVector2(x * f, y * f);
}

_tagVector2 _tagVector2::operator*=(const _tagVector2& v)
{
	x *= v.x;
	y *= v.y;

	return *this;
}

_tagVector2 _tagVector2::operator*=(const XMVECTOR& v)
{
	XMVECTOR	v1 = Convert();
	v1 *= v;

	Convert(v1);

	return *this;
}

_tagVector2 _tagVector2::operator*=(float f)
{
	x *= f;
	y *= f;
	return *this;
}

// /
_tagVector2 _tagVector2::operator/(const _tagVector2& v) const
{
	XMVECTOR	v1 = Convert();
	XMVECTOR	v2 = v.Convert();

	return _tagVector2(v1 / v2);
}

_tagVector2 _tagVector2::operator/(const XMVECTOR& v) const
{
	XMVECTOR	v1 = Convert();

	return _tagVector2(v1 / v);
}

_tagVector2 _tagVector2::operator/(float f) const
{
	return _tagVector2(x / f, y / f);
}

_tagVector2 _tagVector2::operator/=(const _tagVector2& v)
{
	x /= v.x;
	y /= v.y;

	return *this;
}

_tagVector2 _tagVector2::operator/=(const XMVECTOR& v)
{
	XMVECTOR	v1 = Convert();
	v1 /= v;

	Convert(v1);

	return *this;
}

_tagVector2 _tagVector2::operator/=(float f)
{
	x /= f;
	y /= f;
	return *this;
}

XMVECTOR _tagVector2::Convert() const
{
	return XMLoadFloat2((XMFLOAT2*)this);
}

void _tagVector2::Convert(const XMVECTOR& v)
{
	XMStoreFloat2((XMFLOAT2*)this, v);
}

float _tagVector2::Length()	const
{
	return XMVectorGetX(XMVector2Length(Convert()));
}

float _tagVector2::Distance(const _tagVector2& v)	const
{
	_tagVector2	_v = v - *this;
	return _v.Length();
}
