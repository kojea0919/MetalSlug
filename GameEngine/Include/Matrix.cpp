#include "Matrix.h"
#include "Vector3.h"

/*
1 0 0 0
0 1 0 0
0 0 1 0
0 0 0 1
*/
_tagMatrix::_tagMatrix()
{
	m = XMMatrixIdentity();
}

_tagMatrix::_tagMatrix(const _tagMatrix& _m)
{
	m = _m.m;
}

_tagMatrix::_tagMatrix(const XMMATRIX& _m)
{
	m = _m;
}

_tagMatrix::_tagMatrix(Vector4 _v[4])
{
	for (int i = 0; i < 4; ++i)
	{
		v[i] = _v[i];
	}
}

Vector4& _tagMatrix::operator [] (int idx)
{
	return v[idx];
}


_tagMatrix _tagMatrix::operator = (const _tagMatrix& _m)
{
	m = _m.m;
	return *this;
}

_tagMatrix _tagMatrix::operator = (const XMMATRIX& _m)
{
	m = _m;
	return *this;
}

_tagMatrix _tagMatrix::operator = (Vector4 _v[4])
{
	for (int i = 0; i < 4; ++i)
	{
		v[i] = _v[i];
	}

	return *this;
}

_tagMatrix _tagMatrix::operator*(const _tagMatrix& _m) const
{
	return _tagMatrix(m * _m.m);
}

_tagMatrix _tagMatrix::operator*(const XMMATRIX& _m) const
{
	return _tagMatrix(m * _m);
}

_tagMatrix _tagMatrix::operator*(Vector4 _v[4]) const
{
	return _tagMatrix(m * _tagMatrix(_v).m);
}

_tagMatrix _tagMatrix::operator*=(const _tagMatrix& _m)
{
	m *= _m.m;
	return *this;
}

_tagMatrix _tagMatrix::operator*=(const XMMATRIX& _m)
{
	m *= _m;
	return *this;
}

_tagMatrix _tagMatrix::operator*=(Vector4 _v[4])
{
	m *= _tagMatrix(_v).m;
	return *this;
}

void _tagMatrix::Identity()
{
	m = XMMatrixIdentity();
}

void _tagMatrix::Transpose()
{
	m = XMMatrixTranspose(m);
}

void _tagMatrix::Inverse()
{
	m = XMMatrixInverse(&XMMatrixDeterminant(m), m);
}

void _tagMatrix::Scaling(const _tagVector3& _v)
{
	m = XMMatrixScaling(_v.x, _v.y, _v.z);
}

void _tagMatrix::Scaling(float x, float y, float z)
{
	m = XMMatrixScaling(x, y, z);
}

void _tagMatrix::Rotation(const _tagVector3& _v)
{
	XMMATRIX	mX = XMMatrixRotationX(DegreeToRadian(_v.x));
	XMMATRIX	mY = XMMatrixRotationY(DegreeToRadian(_v.y));
	XMMATRIX	mZ = XMMatrixRotationZ(DegreeToRadian(_v.z));

	m = mX * mY * mZ;
}

void _tagMatrix::Rotation(float x, float y, float z)
{
	XMMATRIX	mX = XMMatrixRotationX(DegreeToRadian(x));
	XMMATRIX	mY = XMMatrixRotationY(DegreeToRadian(y));
	XMMATRIX	mZ = XMMatrixRotationZ(DegreeToRadian(z));

	m = mX * mY * mZ;
}

void _tagMatrix::RotationX(float x)
{
	m = XMMatrixRotationX(DegreeToRadian(x));
}

void _tagMatrix::RotationY(float y)
{
	m = XMMatrixRotationY(DegreeToRadian(y));
}

void _tagMatrix::RotationZ(float z)
{
	m = XMMatrixRotationZ(DegreeToRadian(z));
}

void _tagMatrix::RotationAxis(const struct _tagVector3& _v, float fAngle)
{
	m = XMMatrixRotationAxis(_v.Convert(), DegreeToRadian(fAngle));
}

void _tagMatrix::Translation(const _tagVector3& _v)
{
	m = XMMatrixTranslation(_v.x, _v.y, _v.z);
}

void _tagMatrix::Translation(float x, float y, float z)
{
	m = XMMatrixTranslation(x, y, z);
}


_tagMatrix _tagMatrix::StaticIdentity()
{
	return _tagMatrix(XMMatrixIdentity());
}

_tagMatrix _tagMatrix::StaticTranspose(const _tagMatrix& _m)
{
	return _tagMatrix(XMMatrixTranspose(_m.m));
}

_tagMatrix _tagMatrix::StaticInverse(const _tagMatrix& _m)
{
	return _tagMatrix(XMMatrixInverse(&XMMatrixDeterminant(_m.m), _m.m));
}

_tagMatrix _tagMatrix::StaticScaling(const _tagVector3& _v)
{
	return _tagMatrix(XMMatrixScaling(_v.x, _v.y, _v.z));
}

_tagMatrix _tagMatrix::StaticScaling(float x, float y, float z)
{
	return _tagMatrix(XMMatrixScaling(x, y, z));
}

_tagMatrix _tagMatrix::StaticRotation(const _tagVector3& _v)
{
	XMMATRIX	mX = XMMatrixRotationX(DegreeToRadian(_v.x));
	XMMATRIX	mY = XMMatrixRotationY(DegreeToRadian(_v.y));
	XMMATRIX	mZ = XMMatrixRotationZ(DegreeToRadian(_v.z));

	return _tagMatrix(mX * mY * mZ);
}

_tagMatrix _tagMatrix::StaticRotation(float x, float y, float z)
{
	XMMATRIX	mX = XMMatrixRotationX(DegreeToRadian(x));
	XMMATRIX	mY = XMMatrixRotationY(DegreeToRadian(y));
	XMMATRIX	mZ = XMMatrixRotationZ(DegreeToRadian(z));

	return _tagMatrix(mX * mY * mZ);
}

_tagMatrix _tagMatrix::StaticRotationX(float x)
{
	return _tagMatrix(XMMatrixRotationX(DegreeToRadian(x)));
}

_tagMatrix _tagMatrix::StaticRotationY(float y)
{
	return _tagMatrix(XMMatrixRotationY(DegreeToRadian(y)));
}

_tagMatrix _tagMatrix::StaticRotationZ(float z)
{
	return _tagMatrix(XMMatrixRotationZ(DegreeToRadian(z)));
}

_tagMatrix _tagMatrix::StaticTranslation(const _tagVector3& _v)
{
	return _tagMatrix(XMMatrixTranslation(_v.x, _v.y, _v.z));
}

_tagMatrix _tagMatrix::StaticTranslation(float x, float y, float z)
{
	return _tagMatrix(XMMatrixTranslation(x, y, z));
}

