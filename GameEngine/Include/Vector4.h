#pragma once

#include "EngineMath.h"

typedef struct _tagVector4
{
	float	x, y, z, w;

	_tagVector4();
	_tagVector4(float _x, float _y, float _z, float _w);
	_tagVector4(const _tagVector4& v);
	_tagVector4(const XMVECTOR& v);

	// operator =
	_tagVector4& operator = (const _tagVector4& v);
	_tagVector4& operator = (const XMVECTOR& v);
	_tagVector4& operator = (float f);

	// operator ==, !=
	bool operator == (const _tagVector4& v)	const;
	bool operator == (const XMVECTOR& v)	const;
	bool operator != (const _tagVector4& v)	const;
	bool operator != (const XMVECTOR& v)	const;

	// operator []
	float operator [] (const int& idx)	const;
	float& operator [] (const int& idx);

	// operator +
	_tagVector4 operator + (const _tagVector4& v)	const;
	_tagVector4 operator + (const XMVECTOR& v)	const;
	_tagVector4 operator + (float f)	const;

	// operator +=
	_tagVector4 operator += (const _tagVector4& v);
	_tagVector4 operator += (const XMVECTOR& v);
	_tagVector4 operator += (float f);

	// operator -
	_tagVector4 operator - (const _tagVector4& v)	const;
	_tagVector4 operator - (const XMVECTOR& v)	const;
	_tagVector4 operator - (float f)	const;

	// operator -=
	_tagVector4 operator -= (const _tagVector4& v);
	_tagVector4 operator -= (const XMVECTOR& v);
	_tagVector4 operator -= (float f);

	// operator *
	_tagVector4 operator * (const _tagVector4& v)	const;
	_tagVector4 operator * (const XMVECTOR& v)	const;
	_tagVector4 operator * (float f)	const;

	// operator *=
	_tagVector4 operator *= (const _tagVector4& v);
	_tagVector4 operator *= (const XMVECTOR& v);
	_tagVector4 operator *= (float f);

	// operator /
	_tagVector4 operator / (const _tagVector4& v)	const;
	_tagVector4 operator / (const XMVECTOR& v)	const;
	_tagVector4 operator / (float f)	const;

	// operator /=
	_tagVector4 operator /= (const _tagVector4& v);
	_tagVector4 operator /= (const XMVECTOR& v);
	_tagVector4 operator /= (float f);

	XMVECTOR Convert()	const;
	void Convert(const XMVECTOR& v);

	static _tagVector4	Black;
	static _tagVector4	White;
	static _tagVector4	Red;
	static _tagVector4	Green;
	static _tagVector4	Blue;
}Vector4, * PVector4;

