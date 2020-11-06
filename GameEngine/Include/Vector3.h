#pragma once

#include "Matrix.h"

typedef struct _tagVector3
{
	float	x, y, z;

	_tagVector3();
	_tagVector3(float _x, float _y, float _z);
	_tagVector3(const _tagVector3& v);
	_tagVector3(const XMVECTOR& v);

	// operator =
	_tagVector3& operator = (const _tagVector3& v);
	_tagVector3& operator = (const XMVECTOR& v);
	_tagVector3& operator = (float f);

	// operator ==, !=
	bool operator == (const _tagVector3& v)	const;
	bool operator == (const XMVECTOR& v)	const;
	bool operator != (const _tagVector3& v)	const;
	bool operator != (const XMVECTOR& v)	const;

	// operator []
	float operator [] (const int& idx)	const;
	float& operator [] (const int& idx);

	// operator +
	_tagVector3 operator + (const _tagVector3& v)	const;
	_tagVector3 operator + (const XMVECTOR& v)	const;
	_tagVector3 operator + (float f)	const;

	// operator +=
	_tagVector3 operator += (const _tagVector3& v);
	_tagVector3 operator += (const XMVECTOR& v);
	_tagVector3 operator += (float f);

	// operator -
	_tagVector3 operator - (const _tagVector3& v)	const;
	_tagVector3 operator - (const XMVECTOR& v)	const;
	_tagVector3 operator - (float f)	const;

	// operator -=
	_tagVector3 operator -= (const _tagVector3& v);
	_tagVector3 operator -= (const XMVECTOR& v);
	_tagVector3 operator -= (float f);

	// operator *
	_tagVector3 operator * (const _tagVector3& v)	const;
	_tagVector3 operator * (const XMVECTOR& v)	const;
	_tagVector3 operator * (float f)	const;

	// operator *=
	_tagVector3 operator *= (const _tagVector3& v);
	_tagVector3 operator *= (const XMVECTOR& v);
	_tagVector3 operator *= (float f);

	// operator /
	_tagVector3 operator / (const _tagVector3& v)	const;
	_tagVector3 operator / (const XMVECTOR& v)	const;
	_tagVector3 operator / (float f)	const;

	// operator /=
	_tagVector3 operator /= (const _tagVector3& v);
	_tagVector3 operator /= (const XMVECTOR& v);
	_tagVector3 operator /= (float f);

	float Length()	const;
	void Normalize();
	float Distance(const _tagVector3& v)	const;
	float Dot(const _tagVector3& v)	const;
	float Angle(const _tagVector3& v)	const;
	_tagVector3 Cross(const _tagVector3& v)	const;

	// w = 0
	_tagVector3 TransformNormal(const Matrix& _m)	const;
	// w = 1
	_tagVector3 TransformCoord(const Matrix& _m)	const;

	XMVECTOR Convert()	const;
	void Convert(const XMVECTOR& v);

	static float Length(const _tagVector3& v);
	static _tagVector3 Normalize(const _tagVector3& v);
	static float Distance(const _tagVector3& vSrc, const _tagVector3& vDest);

	static _tagVector3	Zero;
	static _tagVector3	One;
	static _tagVector3	Axis[AXIS_END];
}Vector3, *PVector3;

