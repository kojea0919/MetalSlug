#include "Collision.h"
#include "ColliderRect.h"
#include "ColliderSphere2D.h"
#include "ColliderOBB2D.h"
#include "ColliderPixel.h"

bool CCollision::CollisionRectToRect(CColliderRect* pSrc, CColliderRect* pDest)
{
	Vector2 vIntersect;

	if (CollisionRectToRect(vIntersect, pSrc->GetInfo(), pDest->GetInfo()))
	{
		pSrc->m_vImpactPoint = Vector3(vIntersect.x, vIntersect.y, 0.f);
		pDest->m_vImpactPoint = Vector3(vIntersect.x, vIntersect.y, 0.f);

		return true;
	}

	return false;
}

bool CCollision::CollisionRectToRect(Vector2& vImpactPoint, const RectInfo& tSrc, const RectInfo& tDest)
{
	if (tSrc.fL > tDest.fR)
		return false;

	else if (tDest.fL > tSrc.fR)
		return false;

	else if (tSrc.fT < tDest.fB)
		return false;

	else if (tDest.fT < tSrc.fB)
		return false;

	RectInfo	tIntersectRC;

	tIntersectRC.fL = tSrc.fL > tDest.fL ? tSrc.fL : tDest.fL;
	tIntersectRC.fR = tSrc.fR < tDest.fR ? tSrc.fR : tDest.fR;
	tIntersectRC.fT = tSrc.fT > tDest.fT ? tSrc.fT : tDest.fT;
	tIntersectRC.fB = tSrc.fB < tDest.fB ? tSrc.fB : tDest.fB;

	vImpactPoint.x = (tIntersectRC.fL + tIntersectRC.fR) / 2.f;
	vImpactPoint.y = (tIntersectRC.fT + tIntersectRC.fB) / 2.f;

	return true;
}

bool CCollision::CollisionSphere2DToSphere2D(CColliderSphere2D* pSrc, CColliderSphere2D* pDest)
{
	Vector3 vIntersect;

	if (CollisionSphereToSphere(vIntersect, pSrc->GetInfo(), pDest->GetInfo()))
	{
		pSrc->m_vImpactPoint = Vector3(vIntersect.x, vIntersect.y, 0.f);
		pDest->m_vImpactPoint = Vector3(vIntersect.x, vIntersect.y, 0.f);

		return true;
	}

	return false;
}

bool CCollision::CollisionSphereToSphere(Vector3& vImpactPoint, const SphereInfo& tSrc, const SphereInfo& tDest)
{
	float	fDist = tSrc.vCenter.Distance(tDest.vCenter);

	if (fDist > tSrc.fRadius + tDest.fRadius)
		return false;

	// 두 원사이의 교점을 구한다.
	Vector3	vDir = tSrc.vCenter - tDest.vCenter;
	vDir.Normalize();

	Vector3	vPoint1, vPoint2;

	vPoint1 = tDest.vCenter + vDir * tDest.fRadius;

	vDir *= -1.f;

	vPoint2 = tSrc.vCenter + vDir * tSrc.fRadius;

	vDir = vPoint1 - vPoint2;

	fDist = vDir.Length() / 2.f;
	vDir.Normalize();

	vImpactPoint = vPoint2 + vDir * fDist;

	return true;
}

bool CCollision::CollisionRectToSphere2D(CColliderRect* pSrc, CColliderSphere2D* pDest)
{
	Vector3	vIntersect;

	if (CollisionRectToSphere(vIntersect, pSrc->GetInfo(), pDest->GetInfo()))
	{
		pSrc->m_vImpactPoint = vIntersect;
		pDest->m_vImpactPoint = vIntersect;

		return true;
	}

	return false;
}

bool CCollision::CollisionRectToSphere(Vector3& vImpactPoint, const RectInfo& tSrc, const SphereInfo& tDest)
{
	if ((tSrc.fL <= tDest.vCenter.x && tDest.vCenter.x <= tSrc.fR) ||
		(tSrc.fB <= tDest.vCenter.y && tDest.vCenter.y <= tSrc.fT))
	{
		// 사각형을 반지름만큼 확장
		RectInfo	tExtInfo = {};
		tExtInfo.fL = tSrc.fL - tDest.fRadius;
		tExtInfo.fB = tSrc.fB - tDest.fRadius;
		tExtInfo.fR = tSrc.fR + tDest.fRadius;
		tExtInfo.fT = tSrc.fT + tDest.fRadius;

		if (tExtInfo.fL > tDest.vCenter.x)
			return false;

		else if (tExtInfo.fR < tDest.vCenter.x)
			return false;

		else if (tExtInfo.fT < tDest.vCenter.y)
			return false;

		else if (tExtInfo.fB > tDest.vCenter.y)
			return false;

		Vector3	vCenter;
		vCenter.x = (tSrc.fL + tSrc.fR) / 2.f;
		vCenter.y = (tSrc.fT + tSrc.fB) / 2.f;

		Vector3	vDir = vCenter - tDest.vCenter;
		vDir.Normalize();

		vImpactPoint = tDest.vCenter + vDir * tDest.fRadius;

		return true;
	}

	// 사각형의 4개의 꼭지점중 하나라도 원 안에 들어오면 충돌이다.
	Vector3	vVertex[4] =
	{
		Vector3(tSrc.fL, tSrc.fT, 0.f),
		Vector3(tSrc.fR, tSrc.fT, 0.f),
		Vector3(tSrc.fL, tSrc.fB, 0.f),
		Vector3(tSrc.fR, tSrc.fB, 0.f)
	};

	for (int i = 0; i < 4; ++i)
	{
		if (CollisionSphereToPoint(vImpactPoint, tDest, vVertex[i]))
			return true;
	}

	return false;
}

bool CCollision::CollisionOBB2DToOBB2D(CColliderOBB2D* pSrc, CColliderOBB2D* pDest)
{
	Vector3	vIntersect;

	if (CollisionOBB2DToOBB2D(vIntersect, pSrc->GetInfo(),
		pDest->GetInfo()))
	{
		pSrc->m_vImpactPoint = vIntersect;
		pDest->m_vImpactPoint = vIntersect;

		return true;
	}

	return false;
}

bool CCollision::CollisionOBB2DToOBB2D(Vector3& vImpactPoint, const OBBInfo& tSrc, const OBBInfo& tDest)
{
	//Center -> Cetner Vector
	Vector3 CToCVector = tSrc.vCenter - tDest.vCenter;

	//OBB Axis Vector
	//------------------------------------------------------
	vector<Vector3> AllAxis;
	Vector3 Obb1XAxis = tSrc.vAxis[AXIS::AXIS_X];
	AllAxis.push_back(Obb1XAxis);
	Vector3 Obb1YAxis = tSrc.vAxis[AXIS::AXIS_Y];
	AllAxis.push_back(Obb1YAxis);
	Vector3 Obb2XAxis = tDest.vAxis[AXIS::AXIS_X];
	AllAxis.push_back(Obb2XAxis);
	Vector3 Obb2YAxis = tDest.vAxis[AXIS::AXIS_Y];
	AllAxis.push_back(Obb2YAxis);

	Vector3 Obb1UpVector = Obb1YAxis * tSrc.vLength.y;
	Vector3 Obb1RightVector = Obb1XAxis * tSrc.vLength.x;
	Vector3 Obb2UpVector = Obb2YAxis * tDest.vLength.y;
	Vector3 Obb2RightVector = Obb2XAxis * tDest.vLength.x;
	//------------------------------------------------------

	//충돌 판정
	//------------------------------------------------------
	size_t iSize = AllAxis.size();

	for (size_t iCnt = 0; iCnt < iSize; ++iCnt)
	{
		Vector3 CurTargetAxis = AllAxis[iCnt];

		//Center To Center Vector를 현재 축에 정사영
		//----------------------------------------
		Vector3 ProjC2CVec = CurTargetAxis * CurTargetAxis.Dot(CToCVector);
		//----------------------------------------

		//각 Up,Right Vector들도 정사영
		//----------------------------------------
		Vector3 ProjObb1UpVec = CurTargetAxis * CurTargetAxis.Dot(Obb1UpVector);
		Vector3 ProjObb1RightVec = CurTargetAxis * CurTargetAxis.Dot(Obb1RightVector);
		Vector3 ProjObb2UpVec = CurTargetAxis * CurTargetAxis.Dot(Obb2UpVector);
		Vector3 ProjObb2RightVec = CurTargetAxis * CurTargetAxis.Dot(Obb2RightVector);
		//----------------------------------------

		if (ProjC2CVec.Length() > ProjObb1UpVec.Length() + ProjObb1RightVec.Length() +
			ProjObb2UpVec.Length() + ProjObb2RightVec.Length())
		{
			return false;
		}
	}
	//------------------------------------------------------

	return true;
}

bool CCollision::CollisionRectToOBB2D(CColliderRect* pSrc, CColliderOBB2D* pDest)
{
	Vector3	vIntersect;

	RectInfo	tSrcInfo = pSrc->GetInfo();

	OBBInfo	tSrc;
	tSrc.vCenter.x = (tSrcInfo.fL + tSrcInfo.fR) / 2.f;
	tSrc.vCenter.y = (tSrcInfo.fT + tSrcInfo.fB) / 2.f;

	tSrc.vLength.x = (tSrcInfo.fR - tSrcInfo.fL) / 2.f;
	tSrc.vLength.y = (tSrcInfo.fT - tSrcInfo.fB) / 2.f;

	if (CollisionOBB2DToOBB2D(vIntersect, tSrc,
		pDest->GetInfo()))
	{
		pSrc->m_vImpactPoint = vIntersect;
		pDest->m_vImpactPoint = vIntersect;

		return true;
	}

	return false;
}

bool CCollision::CollisionSphere2DToOBB2D(CColliderSphere2D* pSrc, CColliderOBB2D* pDest)
{
	Vector3	vIntersect;

	if (CollisionSphere2DToOBB2D(vIntersect, pSrc->GetInfo(),
		pDest->GetInfo()))
	{
		pSrc->m_vImpactPoint = vIntersect;
		pDest->m_vImpactPoint = vIntersect;

		return true;
	}

	return false;
}

bool CCollision::CollisionSphere2DToOBB2D(Vector3& vImpactPoint, const SphereInfo& tSrc, const OBBInfo& tDest)
{
	Vector3	vLength = tSrc.vCenter - tDest.vCenter;
	Vector3	vAxis = tDest.vAxis[AXIS_X];

	float	fLength = abs(vLength.Dot(vAxis));
	float	r1, r2;

	r1 = tDest.vLength.x;
	r2 = tSrc.fRadius;

	if (fLength > r1 + r2)
		return false;

	vAxis = tDest.vAxis[AXIS_Y];
	fLength = abs(vLength.Dot(vAxis));

	r1 = tDest.vLength.y;

	if (fLength > r1 + r2)
		return false;

	vAxis = vLength;
	vAxis.Normalize();
	fLength = vLength.Length();

	r1 = abs(tDest.vAxis[AXIS_X].Dot(vAxis)) * tDest.vLength.x +
		abs(tDest.vAxis[AXIS_Y].Dot(vAxis)) * tDest.vLength.y;

	if (fLength > r1 + r2)
		return false;

	return true;
}

bool CCollision::CollisionRectToPixel(CColliderRect* pSrc, CColliderPixel* pDest)
{
	Vector3 vIntersect;

	if (CollisionRectToPixel(vIntersect, pSrc->GetInfo(), pDest->GetInfo()))
	{
		pSrc->m_vImpactPoint = vIntersect;
		pDest->m_vImpactPoint = vIntersect;

		return true;
	}

	return false;
}

bool CCollision::CollisionRectToPixel(Vector3& vImpactPoint, const RectInfo& tSrc, const PixelColliderInfo& tDest)
{
	//Rect가 해당 픽셀충돌체의 영역안에 들어왔는지 확인
	//----------------------------------------
	if (tSrc.fL > tDest.tRC.fR)
		return false;

	else if (tDest.tRC.fL > tSrc.fR)
		return false;

	else if (tSrc.fT < tDest.tRC.fB)
		return false;

	else if (tDest.tRC.fT < tSrc.fB)
		return false;
	//----------------------------------------

	RectInfo tResult;
	int iStartX, iEndX, iStartY, iEndY;

	//충돌된 영역 Setting
	//-----------------------------
	iStartX = tSrc.fL > tDest.tRC.fL ? (int)tSrc.fL : (int)tDest.tRC.fL;
	iEndX = tSrc.fR < tDest.tRC.fR ? (int)tSrc.fR : (int)tDest.tRC.fR;
	iEndY = tSrc.fB > tDest.tRC.fB ? (int)tSrc.fB : (int)tDest.tRC.fB;
	iStartY = tSrc.fT < tDest.tRC.fT ? (int)tSrc.fT : (int)tDest.tRC.fT;
	//-----------------------------

	//원점으로 이동(인덱스로 사용하기 위해)
	//-----------------------------
	iStartX -= (int)tDest.tRC.fL;
	iEndX -= (int)tDest.tRC.fL;

	iStartY -= (int)tDest.tRC.fB;
	iEndY -= (int)tDest.tRC.fB;
	//-----------------------------


	//픽셀 충돌 Check
	//----------------------------------------
	for (int iHeightCnt = iStartY; iHeightCnt >= iEndY; --iHeightCnt)
	{
		int iIdxY = tDest.iHeight - 1 - iHeightCnt;

		if (iIdxY < 0 || iIdxY >= tDest.iHeight)
			continue;

		for (int iWidthCnt = iStartX; iWidthCnt <= iEndX; ++iWidthCnt)
		{
			int iIdxX = iWidthCnt;

			if (iIdxX < 0 || iIdxX >= tDest.iWidth)
				continue;

			//실제 Index setting
			int iIndex = iIdxY * tDest.iWidth * 4 + iIdxX * 4;

			//collision_type에 따라 다른 Check
			//--------------------------------------
			switch (tDest.eType)
			{
			case PIXEL_COLLISION_TYPE::Color_Ignore:
				//해당 픽셀이 IgnoreColor인 경우 pass
				if (tDest.pPixel[iIndex] == tDest.IgnoreColor[0] &&
					tDest.pPixel[iIndex + 1] == tDest.IgnoreColor[1] &&
					tDest.pPixel[iIndex + 2] == tDest.IgnoreColor[2])
					continue;

				vImpactPoint.x = iWidthCnt + tDest.tRC.fL;
				vImpactPoint.y = iHeightCnt + tDest.tRC.fB;
				return true;
			case PIXEL_COLLISION_TYPE::Less_Alpha_Ignore:
				//해당 픽셀 알파값이 Setting된 Alpha값보다 작거나 같은경우 pass
				if (tDest.pPixel[iIndex + 3] <= tDest.IgnoreColor[3])
					continue;

				vImpactPoint.x = iWidthCnt + tDest.tRC.fL;
				vImpactPoint.y = iHeightCnt + tDest.tRC.fB;
				return true;
			case PIXEL_COLLISION_TYPE::Color_Confirm:
				//?
				if (tDest.pPixel[iIndex] != tDest.IgnoreColor[0] ||
					tDest.pPixel[iIndex + 1] != tDest.IgnoreColor[1] ||
					tDest.pPixel[iIndex + 2] != tDest.IgnoreColor[2])
					continue;

				vImpactPoint.x = iWidthCnt + tDest.tRC.fL;
				vImpactPoint.y = iHeightCnt + tDest.tRC.fB;
				return true;
			}	
			//--------------------------------------
		}
	}
	//----------------------------------------

	return false;
}

bool CCollision::CollisionSphere2DToPixel(CColliderSphere2D* pSrc, CColliderPixel* pDest)
{
	Vector3	vIntersect;

	if (CollisionSphere2DToPixel(vIntersect,pSrc->GetMin(),
		pSrc->GetMax(), pSrc->GetInfo(),
		pDest->GetInfo()))
	{
		pSrc->m_vImpactPoint = vIntersect;
		pDest->m_vImpactPoint = vIntersect;

		return true;
	}

	return false;
}

bool CCollision::CollisionSphere2DToPixel(Vector3& vImpactPoint, const Vector3& vSrcMin, const Vector3& vSrcMax, const SphereInfo& tSrc, const PixelColliderInfo& tDest)
{
	//Sphere -> Rect으로 변환
	//----------------------
	RectInfo	tSrcRC;
	tSrcRC.fL = vSrcMin.x;
	tSrcRC.fB = vSrcMin.y;
	tSrcRC.fR = vSrcMax.x;
	tSrcRC.fT = vSrcMax.y;
	//----------------------

	//Rect가 해당 픽셀충돌체의 영역안에 들어왔는지 확인
	//----------------------------------------
	if (tSrcRC.fL > tDest.tRC.fR)
		return false;

	else if (tDest.tRC.fL > tSrcRC.fR)
		return false;

	else if (tSrcRC.fT < tDest.tRC.fB)
		return false;

	else if (tDest.tRC.fT < tSrcRC.fB)
		return false;
	//----------------------------------------

	RectInfo tResult;
	int iStartX, iEndX, iStartY, iEndY;

	//충돌된 영역 Setting
	//-----------------------------
	iStartX = tSrcRC.fL > tDest.tRC.fL ? (int)tSrcRC.fL : (int)tDest.tRC.fL;
	iEndX = tSrcRC.fR < tDest.tRC.fR ? (int)tSrcRC.fR : (int)tDest.tRC.fR;
	iEndY = tSrcRC.fB > tDest.tRC.fB ? (int)tSrcRC.fB : (int)tDest.tRC.fB;
	iStartY = tSrcRC.fT < tDest.tRC.fT ? (int)tSrcRC.fT : (int)tDest.tRC.fT;
	//-----------------------------

	//원점으로 이동(인덱스로 사용하기 위해)
	//-----------------------------
	iStartX -= (int)tDest.tRC.fL;
	iEndX -= (int)tDest.tRC.fL;

	iStartY -= (int)tDest.tRC.fB;
	iEndY -= (int)tDest.tRC.fB;
	//-----------------------------


	//픽셀 충돌 Check
	//----------------------------------------
	for (int iHeightCnt = iStartY; iHeightCnt >= iEndY; --iHeightCnt)
	{
		int iIdxY = tDest.iHeight - 1 - iHeightCnt;

		if (iIdxY < 0 || iIdxY >= tDest.iHeight)
			continue;

		for (int iWidthCnt = iStartX; iWidthCnt <= iEndX; ++iWidthCnt)
		{
			int iIdxX = iWidthCnt;

			if (iIdxX < 0 || iIdxX >= tDest.iWidth)
				continue;

			//실제 Index setting
			int iIndex = iIdxY * tDest.iWidth * 4 + iIdxX * 4;

			bool bCollision = false;

			//collision_type에 따라 다른 Check
			//--------------------------------------
			switch (tDest.eType)
			{
			case PIXEL_COLLISION_TYPE::Color_Ignore:
				//해당 픽셀이 IgnoreColor인 경우 pass
				if (tDest.pPixel[iIndex] == tDest.IgnoreColor[0] &&
					tDest.pPixel[iIndex + 1] == tDest.IgnoreColor[1] &&
					tDest.pPixel[iIndex + 2] == tDest.IgnoreColor[2])
					continue;

				bCollision = true;
				break;
			case PIXEL_COLLISION_TYPE::Less_Alpha_Ignore:
				//해당 픽셀 알파값이 Setting된 Alpha값보다 작거나 같은경우 pass
				if (tDest.pPixel[iIndex + 3] <= tDest.IgnoreColor[3])
					continue;

				bCollision = true;
				break;
			case PIXEL_COLLISION_TYPE::Color_Confirm:
				//?
				if (tDest.pPixel[iIndex] != tDest.IgnoreColor[0] ||
					tDest.pPixel[iIndex + 1] != tDest.IgnoreColor[1] ||
					tDest.pPixel[iIndex + 2] != tDest.IgnoreColor[2])
					continue;

				bCollision = true;
				break;
			}
			//--------------------------------------

			//충돌이되면 OBB영역 안에있는지 확인
			//------------------------------
			Vector3	vPoint;
			vPoint.x = iWidthCnt + tDest.tRC.fL;
			vPoint.y = iHeightCnt + tDest.tRC.fB;

			if (bCollision && CollisionSphereToPoint(vImpactPoint,
				tSrc, vPoint))
				return true;
			//------------------------------
		}
	}
	//----------------------------------------

	return false;
}

bool CCollision::CollisionOBB2DToPixel(CColliderOBB2D* pSrc, CColliderPixel* pDest)
{
	Vector3	vIntersect;

	if (CollisionOBB2DToPixel(vIntersect,pSrc->GetMin(),
		pSrc->GetMax(), pSrc->GetInfo(),
		pDest->GetInfo()))
	{
		pSrc->m_vImpactPoint = vIntersect;
		pDest->m_vImpactPoint = vIntersect;

		return true;
	}

	return false;
}

bool CCollision::CollisionOBB2DToPixel(Vector3& vImpactPoint, const Vector3& vSrcMin, const Vector3& vSrcMax, const OBBInfo& tSrc, const PixelColliderInfo& tDest)
{
	//OBB -> Rect으로 변환
	//----------------------
	RectInfo	tSrcRC;
	tSrcRC.fL = vSrcMin.x;
	tSrcRC.fB = vSrcMin.y;
	tSrcRC.fR = vSrcMax.x;
	tSrcRC.fT = vSrcMax.y;
	//----------------------

	//Rect가 해당 픽셀충돌체의 영역안에 들어왔는지 확인
	//----------------------------------------
	if (tSrcRC.fL > tDest.tRC.fR)
		return false;

	else if (tDest.tRC.fL > tSrcRC.fR)
		return false;

	else if (tSrcRC.fT < tDest.tRC.fB)
		return false;

	else if (tDest.tRC.fT < tSrcRC.fB)
		return false;
	//----------------------------------------

	RectInfo tResult;
	int iStartX, iEndX, iStartY, iEndY;

	//충돌된 영역 Setting
	//-----------------------------
	iStartX = tSrcRC.fL > tDest.tRC.fL ? (int)tSrcRC.fL : (int)tDest.tRC.fL;
	iEndX = tSrcRC.fR < tDest.tRC.fR ? (int)tSrcRC.fR : (int)tDest.tRC.fR;
	iEndY = tSrcRC.fB > tDest.tRC.fB ? (int)tSrcRC.fB : (int)tDest.tRC.fB;
	iStartY = tSrcRC.fT < tDest.tRC.fT ? (int)tSrcRC.fT : (int)tDest.tRC.fT;
	//-----------------------------

	//원점으로 이동(인덱스로 사용하기 위해)
	//-----------------------------
	iStartX -= (int)tDest.tRC.fL;
	iEndX -= (int)tDest.tRC.fL;

	iStartY -= (int)tDest.tRC.fB;
	iEndY -= (int)tDest.tRC.fB;
	//-----------------------------


	//픽셀 충돌 Check
	//----------------------------------------
	for (int iHeightCnt = iStartY; iHeightCnt >= iEndY; --iHeightCnt)
	{
		int iIdxY = tDest.iHeight - 1 - iHeightCnt;

		if (iIdxY < 0 || iIdxY >= tDest.iHeight)
			continue;

		for (int iWidthCnt = iStartX; iWidthCnt <= iEndX; ++iWidthCnt)
		{
			int iIdxX = iWidthCnt;

			if (iIdxX < 0 || iIdxX >= tDest.iWidth)
				continue;

			//실제 Index setting
			int iIndex = iIdxY * tDest.iWidth * 4 + iIdxX * 4;

			bool bCollision = false;

			//collision_type에 따라 다른 Check
			//--------------------------------------
			switch (tDest.eType)
			{
			case PIXEL_COLLISION_TYPE::Color_Ignore:
				//해당 픽셀이 IgnoreColor인 경우 pass
				if (tDest.pPixel[iIndex] == tDest.IgnoreColor[0] &&
					tDest.pPixel[iIndex + 1] == tDest.IgnoreColor[1] &&
					tDest.pPixel[iIndex + 2] == tDest.IgnoreColor[2])
					continue;

				bCollision = true;
				break;
			case PIXEL_COLLISION_TYPE::Less_Alpha_Ignore:
				//해당 픽셀 알파값이 Setting된 Alpha값보다 작거나 같은경우 pass
				if (tDest.pPixel[iIndex + 3] <= tDest.IgnoreColor[3])
					continue;

				bCollision = true;
				break;
			case PIXEL_COLLISION_TYPE::Color_Confirm:
				//?
				if (tDest.pPixel[iIndex] != tDest.IgnoreColor[0] ||
					tDest.pPixel[iIndex + 1] != tDest.IgnoreColor[1] ||
					tDest.pPixel[iIndex + 2] != tDest.IgnoreColor[2])
					continue;

				bCollision = true;
				break;
			}
			//--------------------------------------

			//충돌이되면 OBB영역 안에있는지 확인
			//------------------------------
			Vector3	vPoint;
			vPoint.x = iWidthCnt + tDest.tRC.fL;
			vPoint.y = iHeightCnt + tDest.tRC.fB;

			if (bCollision && CollisionOBB2DToPoint(vImpactPoint,
				tSrc, vPoint))
				return true;
			//------------------------------
		}
	}
	//----------------------------------------

	return false;
}

bool CCollision::CollisionRectToPoint(Vector3& vImpactPoint, const RectInfo& tSrc, const Vector3& vDest)
{
	if (tSrc.fL > vDest.x)
		return false;

	else if (tSrc.fR < vDest.x)
		return false;

	else if (tSrc.fT < vDest.y)
		return false;

	else if (tSrc.fB > vDest.y)
		return false;

	vImpactPoint.x = vDest.x;
	vImpactPoint.y = vDest.y;

	return true;
}

bool CCollision::CollisionSphereToPoint(Vector3& vImpactPoint, const SphereInfo& tSrc, const Vector3& vDest)
{
	float	fDist = tSrc.vCenter.Distance(vDest);

	if (fDist <= tSrc.fRadius)
	{
		vImpactPoint = vDest;
		return true;
	}

	return false;
}

bool CCollision::CollisionOBB2DToPoint(Vector3& vImpactPoint, const OBBInfo& tSrc, const Vector3& vDest)
{
	Vector3	vLength = tSrc.vCenter - vDest;
	Vector3	vAxis = tSrc.vAxis[AXIS_X];

	float	fLength = abs(vLength.Dot(vAxis));

	if (fLength > tSrc.vLength.x)
		return false;

	vAxis = tSrc.vAxis[AXIS_Y];

	fLength = abs(vLength.Dot(vAxis));

	if (fLength > tSrc.vLength.y)
		return false;

	vImpactPoint = vDest;

	return true;
}

bool CCollision::CollisionLineToLine(Vector2& vImpactPoint, const Vector2& tSrc1, const Vector2& tSrc2, const Vector2& tDest1, const Vector2& tDest2)
{	
	if (tSrc2.x - tSrc1.x == 0)
		return false;

	if (tDest2.x - tDest1.x == 0)
		return false;

	//각 선분의 기울기 setting
	//---------------------------------------------------------
	float fSlope1 = (tSrc2.y - tSrc1.y) / (tSrc2.x - tSrc1.x);
	float fSlope2 = (tDest2.y - tDest1.y) / (tDest2.x - tDest1.x);
	//---------------------------------------------------------

	//각 선분의 y절편(Intercept)
	//---------------------------------------------------------
	float fIntercept1 = -fSlope1 * tSrc1.x + tSrc1.y;
	float fIntercept2 = -fSlope2 * tSrc2.x + tSrc2.y;
	//---------------------------------------------------------

	//같은 선분인 경우(기울기와 절편이 같은 경우)
	if (fSlope1 == fSlope2 && fIntercept1 == fIntercept2)
	{
		vImpactPoint = tSrc1;
		return true;
	}
	//평행한 경우(절편이 다른 경우)
	else if (fSlope1 == fSlope2 && fIntercept1 != fIntercept2)
	{
		return false;
	}
	else
	{
		/*float fBigX = tDest2.x > tSrc2.x ? tSrc2.x : tDest2.x;
		float fSmallX = tDest1.x > tSrc1.x ? tSrc1.x : tDest1.x;*/

		float fValue = (fIntercept1 - fIntercept2) / (fSlope2 - fSlope1);

		/*if (fValue <= tSrc && fValue >= fSmallX)
			return true;
		else
			return false;*/
	}

	return false;
}
