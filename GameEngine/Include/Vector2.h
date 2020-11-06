#pragma once

#include "EngineMath.h"

typedef struct _tagVector2
{
	float	x, y;

	_tagVector2();
	_tagVector2(float _x, float _y);
	_tagVector2(const _tagVector2& v);
	_tagVector2(const XMVECTOR& v);

	// operator =
	_tagVector2& operator = (const _tagVector2& v);
	_tagVector2& operator = (const XMVECTOR& v);
	_tagVector2& operator = (float f);

	// operator ==, !=
	bool operator == (const _tagVector2& v)	const;
	bool operator == (const XMVECTOR& v)	const;
	bool operator != (const _tagVector2& v)	const;
	bool operator != (const XMVECTOR& v)	const;

	// operator []
	float operator [] (const int& idx)	const;
	float& operator [] (const int& idx);

	// operator +
	_tagVector2 operator + (const _tagVector2& v)	const;
	_tagVector2 operator + (const XMVECTOR& v)	const;
	_tagVector2 operator + (float f)	const;

	// operator +=
	_tagVector2 operator += (const _tagVector2& v);
	_tagVector2 operator += (const XMVECTOR& v);
	_tagVector2 operator += (float f);

	// operator -
	_tagVector2 operator - (const _tagVector2& v)	const;
	_tagVector2 operator - (const XMVECTOR& v)	const;
	_tagVector2 operator - (float f)	const;

	// operator -=
	_tagVector2 operator -= (const _tagVector2& v);
	_tagVector2 operator -= (const XMVECTOR& v);
	_tagVector2 operator -= (float f);

	// operator *
	_tagVector2 operator * (const _tagVector2& v)	const;
	_tagVector2 operator * (const XMVECTOR& v)	const;
	_tagVector2 operator * (float f)	const;

	// operator *=
	_tagVector2 operator *= (const _tagVector2& v);
	_tagVector2 operator *= (const XMVECTOR& v);
	_tagVector2 operator *= (float f);

	// operator /
	_tagVector2 operator / (const _tagVector2& v)	const;
	_tagVector2 operator / (const XMVECTOR& v)	const;
	_tagVector2 operator / (float f)	const;

	// operator /=
	_tagVector2 operator /= (const _tagVector2& v);
	_tagVector2 operator /= (const XMVECTOR& v);
	_tagVector2 operator /= (float f);

	XMVECTOR Convert()	const;
	void Convert(const XMVECTOR& v);
	float Length()	const;
	float Distance(const _tagVector2& v)	const;
}Vector2, * PVector2;
