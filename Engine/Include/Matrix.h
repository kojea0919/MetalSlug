#pragma once

#include "Vector4.h"

typedef union __declspec(align(16)) _tagMatrix
{
	XMMATRIX	m;

	struct
	{
		float	_11, _12, _13, _14;
		float	_21, _22, _23, _24;
		float	_31, _32, _33, _34;
		float	_41, _42, _43, _44;
	};

	Vector4	v[4];

	_tagMatrix();
	_tagMatrix(const _tagMatrix& _m);
	_tagMatrix(const XMMATRIX& _m);
	_tagMatrix(Vector4 _v[4]);

	Vector4& operator [] (int idx);
	_tagMatrix operator = (const _tagMatrix& _m);
	_tagMatrix operator = (const XMMATRIX& _m);
	_tagMatrix operator = (Vector4 _v[4]);

	_tagMatrix operator * (const _tagMatrix& _m)	const;
	_tagMatrix operator * (const XMMATRIX& _m)	const;
	_tagMatrix operator * (Vector4 _v[4])	const;

	_tagMatrix operator *= (const _tagMatrix& _m);
	_tagMatrix operator *= (const XMMATRIX& _m);
	_tagMatrix operator *= (Vector4 _v[4]);

	/*
	1 2
	3 4

	1 3
	2 4
	*/
	void Identity();
	void Transpose();
	// 행렬A * 행렬A역행렬 = 항등행렬(단위행렬)
	void Inverse();
	void Scaling(const struct _tagVector3& _v);
	void Scaling(float x, float y, float z);
	void Rotation(const struct _tagVector3& _v);
	void Rotation(float x, float y, float z);
	void RotationX(float x);
	void RotationY(float y);
	void RotationZ(float z);
	void RotationAxis(const struct _tagVector3& _v, float fAngle);
	void Translation(const struct _tagVector3& _v);
	void Translation(float x, float y, float z);

	static _tagMatrix StaticIdentity();
	static _tagMatrix StaticTranspose(const _tagMatrix& _m);
	static _tagMatrix StaticInverse(const _tagMatrix& _m);
	static _tagMatrix StaticScaling(const struct _tagVector3& _v);
	static _tagMatrix StaticScaling(float x, float y, float z);
	static _tagMatrix StaticRotation(const struct _tagVector3& _v);
	static _tagMatrix StaticRotation(float x, float y, float z);
	static _tagMatrix StaticRotationX(float x);
	static _tagMatrix StaticRotationY(float y);
	static _tagMatrix StaticRotationZ(float z);
	static _tagMatrix StaticTranslation(const struct _tagVector3& _v);
	static _tagMatrix StaticTranslation(float x, float y, float z);
}Matrix, * PMatrix;