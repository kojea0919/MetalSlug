#pragma once

#include "../GameEngine.h"

class CCollision
{
public:
	//사각형 : 사각형 충돌
	//-------------------------------------------------------
	static bool CollisionRectToRect(class CColliderRect* pSrc,
		class CColliderRect* pDest);

	static bool CollisionRectToRect(Vector2& vImpactPoint, const RectInfo & tSrc,
		const RectInfo& tDest);
	//-------------------------------------------------------

	//구 : 구 충돌
	//-------------------------------------------------------
	static bool CollisionSphere2DToSphere2D(class CColliderSphere2D* pSrc,
		class CColliderSphere2D* pDest);
	static bool CollisionSphereToSphere(Vector3& vImpactPoint, const SphereInfo& tSrc,
		const SphereInfo& tDest);
	//-------------------------------------------------------

	//구 : 사각형 충돌
	//-------------------------------------------------------
	static bool CollisionRectToSphere2D(class CColliderRect* pSrc,
		class CColliderSphere2D* pDest);
	static bool CollisionRectToSphere(Vector3& vImpactPoint, const RectInfo& tSrc,
		const SphereInfo& tDest);
	//-------------------------------------------------------

public:
	//OBB 충돌
	//-------------------------------------------------------
	static bool CollisionOBB2DToOBB2D(class CColliderOBB2D* pSrc,
		class CColliderOBB2D* pDest);
	static bool CollisionOBB2DToOBB2D(Vector3& vImpactPoint,
		const OBBInfo& tSrc, const OBBInfo& tDest);

	static bool CollisionRectToOBB2D(class CColliderRect* pSrc,
		class CColliderOBB2D* pDest);

	static bool CollisionSphere2DToOBB2D(class CColliderSphere2D* pSrc,
		class CColliderOBB2D* pDest);
	static bool CollisionSphere2DToOBB2D(Vector3& vImpactPoint,
		const SphereInfo& tSrc, const OBBInfo& tDest);
	//-------------------------------------------------------

	//픽셀 충돌
	//-------------------------------------------------------
	static bool CollisionRectToPixel(class CColliderRect* pSrc,
		class CColliderPixel* pDest);
	static bool CollisionRectToPixel(Vector3& vImpactPoint,
		const RectInfo& tSrc, const PixelColliderInfo& tDest);

	static bool CollisionSphere2DToPixel(class CColliderSphere2D* pSrc,
		class CColliderPixel* pDest);
	static bool CollisionSphere2DToPixel(Vector3& vImpactPoint,
		const Vector3& vSrcMin, const Vector3& vSrcMax,
		const SphereInfo& tSrc, const PixelColliderInfo& tDest);

	static bool CollisionOBB2DToPixel(class CColliderOBB2D* pSrc,
		class CColliderPixel* pDest);
	static bool CollisionOBB2DToPixel(Vector3& vImpactPoint,
		const Vector3& vSrcMin, const Vector3& vSrcMax,
		const OBBInfo& tSrc, const PixelColliderInfo& tDest);
	//-------------------------------------------------------

public:
	//사각형 : 점 충돌
	//-------------------------------------------------------
	static bool CollisionRectToPoint(Vector3& vImpactPoint, const RectInfo& tSrc,
		const Vector3& vDest);
	//-------------------------------------------------------

	//구 : 점 충돌
	//-------------------------------------------------------
	static bool CollisionSphereToPoint(Vector3& vImpactPoint, const SphereInfo& tSrc,
		const Vector3& vDest);
	//-------------------------------------------------------

	//OBB : 점 충돌
	//-------------------------------------------------------
	static bool CollisionOBB2DToPoint(Vector3& vImpactPoint, const OBBInfo& tSrc,
		const Vector3& vDest);
	//-------------------------------------------------------
public:
	//선 : 선 충돌
	//-------------------------------------------------------
	static bool CollisionLineToLine(Vector2& vImpactPoint, const Vector2& tSrc1,
		const Vector2& tSrc2,const Vector2& tDest1,const Vector2& tDest2);
	//-------------------------------------------------------
	//-------------------------------------------------------
};

