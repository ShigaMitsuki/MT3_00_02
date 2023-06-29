#include "Polygon.h"
#include<Novice.h>
#define _USE_MATH_DEFINES
#include<Math.h>
#include<algorithm>
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {

	const float kGridHalfWidth = 2.0f;
	const uint32_t kSubdivision = 10;
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision);

	unsigned int color = 0xAAAAAAFF;

	for (int xIndex = 0; xIndex <= kSubdivision; ++xIndex) {

		Vector3 LinePos = {kGridEvery * (xIndex - 5) , 0.0f ,kGridHalfWidth };
		Vector3 Line2Pos = {kGridEvery * (xIndex - 5), 0.0f ,-kGridHalfWidth };


		Vector3 ndcVertex = Transform(LinePos, viewProjectionMatrix);
		Vector3 ScreenVertices = Transform(ndcVertex, viewportMatrix);

		Vector3 ndcVertex2 = Transform(Line2Pos, viewProjectionMatrix);
		Vector3 ScreenVertices2 = Transform(ndcVertex2, viewportMatrix);
		Novice::DrawLine(
			int(ScreenVertices.x), int(ScreenVertices.y),
			int(ScreenVertices2.x), int(ScreenVertices2.y),

			color

		);

		

		//Novice::ScreenPrintf(0, 20 * xIndex, "%0.2f,%0.2f,%0.2f", LinePos.x, LinePos.y, LinePos.z) ;

		//Novice::ScreenPrintf(0, 20 * xIndex, "%0.2f,%0.2f", ScreenVertices.x, ScreenVertices.y);
		//Novice::ScreenPrintf(150, 20 * xIndex, "%0.2f,%0.2f", ScreenVertices2.x, ScreenVertices2.y);
	}

	for (int zIndex = 0; zIndex <= kSubdivision; ++zIndex) {

		Vector3 LinePos = { kGridHalfWidth,0.0f ,kGridEvery * (zIndex - 5) };
		Vector3 Line2Pos = { -kGridHalfWidth,0.0f ,kGridEvery * (zIndex - 5) };


		Vector3 ndcVertex = Transform(LinePos, viewProjectionMatrix);
		Vector3 ScreenVertices = Transform(ndcVertex, viewportMatrix);

		Vector3 ndcVertex2 = Transform(Line2Pos, viewProjectionMatrix);
		Vector3 ScreenVertices2 = Transform(ndcVertex2, viewportMatrix);
		Novice::DrawLine(
			int(ScreenVertices.x), int(ScreenVertices.y),
			int(ScreenVertices2.x), int(ScreenVertices2.y),

			color

		);
	}
}

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewPortMatrix, uint32_t color)
{
	const uint32_t kSubdivision = 10;
	const float  kLomEvery = 2.0f * float(M_PI) / kSubdivision;
	const float kLatEvery = 2.0f * float(M_PI) / kSubdivision;

	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -float(M_PI) / 2.0f + kLatEvery * latIndex;

		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLomEvery;

			float theta = float(M_PI) / kSubdivision;
			float phi = 2.0f * float(M_PI) / kSubdivision;

			Vector3 a{};
			Vector3 b{};
			Vector3 c{};

			a.x = sphere.radius * cosf(lat) * cosf(lon) + sphere.center.x;
			a.y = sphere.radius * sinf(lat) + sphere.center.y;
			a.z = sphere.radius * cosf(lat) * sinf(lon) + sphere.center.z;

			b.x = sphere.radius * cosf(lat + theta) * cosf(lon) + sphere.center.x;
			b.y = sphere.radius * sinf(lat + theta) + sphere.center.y;
			b.z = sphere.radius * cosf(lat + theta) * sinf(lon) + sphere.center.z;

			c.x = sphere.radius * cosf(lat) * cosf(lon + phi) + sphere.center.x;
			c.y = sphere.radius * sinf(lat) + sphere.center.y;
			c.z = sphere.radius * cosf(lat) * sinf(lon + phi) + sphere.center.z;


			Vector3 ndcVecterxA = Transform(a, viewProjectionMatrix);
			Vector3 ScreenA = Transform(ndcVecterxA , viewPortMatrix);

			Vector3 ndcVecterxB = Transform(b, viewProjectionMatrix);
			Vector3 ScreenB = Transform(ndcVecterxB, viewPortMatrix);

			Vector3 ndcVecterxC = Transform(c, viewProjectionMatrix);
			Vector3 ScreenC = Transform(ndcVecterxC, viewPortMatrix);

			Novice::DrawLine( 
				int(ScreenA.x), int(ScreenA.y),
				int(ScreenB.x), int(ScreenB.y),
				color);

			Novice::DrawLine(
				int(ScreenA.x), int(ScreenA.y),
				int(ScreenC.x), int(ScreenC.y),
				color);
		}
	}


}

void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewPortMatrix, uint32_t color)
{
	Vector3 center = Multiply(plane.distance, plane.normal);
	Vector3 perpendiculars[4];

	perpendiculars[0] = Normalize(Perpendicular(plane.normal));
	perpendiculars[1] = { -perpendiculars[0].x,-perpendiculars[0].y,-perpendiculars[0].z };
	perpendiculars[2] = Cross(plane.normal, perpendiculars[0]);
	perpendiculars[3] = { -perpendiculars[2].x , -perpendiculars[2].y, -perpendiculars[2].z };

	Vector3 points[4];

	for (int32_t index = 0; index < 4; ++index) {
		Vector3 extend = Multiply(2.0f,perpendiculars[index]);
		Vector3 point = Add(center, extend);
		points[index] = Transform(Transform(point, viewProjectionMatrix), viewPortMatrix);
	}
	
	Novice::DrawLine(int(points[0].x), int(points[0].y), int(points[3].x), int(points[3].y), color);
	Novice::DrawLine(int(points[1].x), int(points[1].y), int(points[3].x), int(points[3].y), color);
	Novice::DrawLine(int(points[2].x), int(points[2].y), int(points[1].x), int(points[1].y), color);
	Novice::DrawLine(int(points[2].x), int(points[2].y), int(points[0].x), int(points[0].y), color);
}

void DrawLine(const Segment& segment, const Matrix4x4& viewProjection, const Matrix4x4& viewPortMatrix, uint32_t color)
{
	Vector3 Pos1 = {0.0f,0.0f,0.0f};
	Vector3 Pos2 = { 0.0f,0.0f,0.0f };

	Pos1 = Transform(Transform(segment.origin, viewProjection), viewPortMatrix);
	Pos2 = Transform(Transform(Add(segment.origin, segment.diff), viewProjection), viewPortMatrix);

	Novice::DrawLine(int(Pos1.x), int(Pos1.y), (int)Pos2.x, (int)Pos2.y, color);
}

void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjection, const Matrix4x4& viewPortMatrix, uint32_t color)
{
	Vector3 Pos[3]{};

	for (int i = 0; i < 3; i++) {
		Pos[i] = Transform(Transform(triangle.vertices[i], viewProjection), viewPortMatrix);
	}

	Novice::DrawTriangle(
		(int)Pos[0].x, (int)Pos[0].y,
		(int)Pos[1].x, (int)Pos[1].y,
		(int)Pos[2].x, (int)Pos[2].y,
		color, kFillModeWireFrame

	);

}

void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjection, const Matrix4x4& viewPortMatrix, uint32_t color)
{

	Segment minXmaxXminYminZ{
		{aabb.min.x,aabb.min.y,aabb.min.z},
		Subtract({aabb.max.x,aabb.min.y,aabb.min.z},minXmaxXminYminZ.origin)
	};

	Segment minXmaxXmaxYminZ{
		{aabb.min.x,aabb.max.y,aabb.min.z},
		Subtract({aabb.max.x,aabb.max.y,aabb.min.z},minXmaxXmaxYminZ.origin)
	};

	Segment minXmaxXminYmaxZ{
		{aabb.min.x,aabb.min.y,aabb.max.z},
		Subtract({aabb.max.x,aabb.min.y,aabb.max.z},minXmaxXminYmaxZ.origin)
	};

	Segment minXmaxXmaxYmaxZ{
		{aabb.min.x,aabb.max.y,aabb.max.z},
		Subtract({aabb.max.x,aabb.max.y,aabb.max.z},minXmaxXmaxYmaxZ.origin)
	};

	Segment minYmaxYminXminZ{
		{aabb.min.x,aabb.min.y,aabb.min.z},
		Subtract({aabb.min.x,aabb.max.y,aabb.min.z},minYmaxYminXminZ.origin)
	};

	Segment minYmaxYmaxXminZ{
		{aabb.max.x,aabb.min.y,aabb.min.z},
		Subtract({aabb.max.x,aabb.max.y,aabb.min.z},minYmaxYmaxXminZ.origin)
	};

	Segment minYmaxYminXmaxZ{
		{aabb.min.x,aabb.min.y,aabb.max.z},
		Subtract({aabb.min.x,aabb.max.y,aabb.max.z},minYmaxYminXmaxZ.origin)
	};

	Segment minYmaxYmaxXmaxZ{
		{aabb.max.x,aabb.min.y,aabb.max.z},
		Subtract({aabb.max.x,aabb.max.y,aabb.max.z},minYmaxYmaxXmaxZ.origin)
	};

	Segment minZmaxZminXminY{
		{aabb.min.x,aabb.min.y,aabb.min.z},
		Subtract({aabb.min.x,aabb.min.y,aabb.max.z},minZmaxZminXminY.origin)
	};

	Segment minZmaxZmaxXminY{
		{aabb.max.x,aabb.min.y,aabb.min.z},
		Subtract({aabb.max.x,aabb.min.y,aabb.max.z},minZmaxZmaxXminY.origin)
	};

	Segment minZmaxZminXmaxY{
		{aabb.min.x,aabb.max.y,aabb.min.z},
		Subtract({aabb.min.x,aabb.max.y,aabb.max.z},minZmaxZminXmaxY.origin)
	};

	Segment minZmaxZmaxXmaxY{
		{aabb.max.x,aabb.max.y,aabb.min.z},
		Subtract({aabb.max.x,aabb.max.y,aabb.max.z},minZmaxZmaxXmaxY.origin)
	};


	DrawLine(minXmaxXminYminZ, viewProjection, viewPortMatrix, color);
	DrawLine(minXmaxXmaxYminZ, viewProjection, viewPortMatrix, color);
	DrawLine(minXmaxXminYmaxZ, viewProjection, viewPortMatrix, color);
	DrawLine(minXmaxXmaxYmaxZ, viewProjection, viewPortMatrix, color);

	DrawLine(minYmaxYminXminZ, viewProjection, viewPortMatrix, color);
	DrawLine(minYmaxYmaxXminZ, viewProjection, viewPortMatrix, color);
	DrawLine(minYmaxYminXmaxZ, viewProjection, viewPortMatrix, color);
	DrawLine(minYmaxYmaxXmaxZ, viewProjection, viewPortMatrix, color);

	DrawLine(minZmaxZminXminY, viewProjection, viewPortMatrix, color);
	DrawLine(minZmaxZmaxXminY, viewProjection, viewPortMatrix, color);
	DrawLine(minZmaxZminXmaxY, viewProjection, viewPortMatrix, color);
	DrawLine(minZmaxZmaxXmaxY, viewProjection, viewPortMatrix, color);
}

bool IsCollision(const Sphere& SphereA, const Sphere& SphereB)
{
	float distance = Length(Subtract(SphereA.center, SphereB.center));
	if (distance <= SphereA.radius + SphereB.radius) {
		return true;
	}
	return false;
}

bool IsCollision(const Sphere& sphere, const Plane& plane)
{
	float distance = fabsf(Dot(plane.normal, sphere.center) - plane.distance);

	if (distance <= sphere.radius) {
		return true;
	}

	return false;
}

bool IsCollision(const Segment& segment, const Plane& plane)
{
	float dot = Dot(plane.normal, segment.diff);

	if (dot == 0.0f) {
		return false;
	}
	
	float t = (plane.distance - Dot(segment.origin, plane.normal)) / dot;

	if (t >= 0.0f && t <= 1.0f) {
		return true;
	}

	return false;
}

bool IsCollision(const Segment& segment, const Triangle& triangle)
{

	Vector3 v01 = Subtract(triangle.vertices[1], triangle.vertices[0]);
	Vector3 v12 = Subtract(triangle.vertices[2], triangle.vertices[1]);
	Vector3 v20 = Subtract(triangle.vertices[0], triangle.vertices[2]);

	Vector3 n = Cross(v01,v12);

	n = Normalize(n);

	float distance = Dot(triangle.vertices[0], n);

	float dot = Dot(n, segment.diff);

	if (dot == 0.0f) {
		return false;
	}



	float t = (distance - Dot(segment.diff, n)) / dot;

	//if (t >= 0.0f && t <= 1.0f) {
	//	return true;
	//}

	if (t < 0.0f || t > 1.0f) {
		//return true;
		return false;
	}

	Vector3 point = Add(segment.origin, Multiply(t, segment.diff));

	Vector3 v0p = Subtract(point, triangle.vertices[0]);
	Vector3 v1p = Subtract(point, triangle.vertices[1]);
	Vector3 v2p = Subtract(point, triangle.vertices[2]);

	Vector3 cross01 = Cross(v01, v1p);
	Vector3 cross12 = Cross(v12, v2p);
	Vector3 cross20 = Cross(v20, v0p);

	if (Dot(cross01,n) >= 0.0f && Dot(cross12, n) >= 0.0f && Dot(cross20, n) >= 0.0f)  {

		return true;
	}

	return false;
}

bool IsCollision(const AABB& aabb1, const AABB& aabb2)
{
	if ((aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) &&
		(aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) &&
		(aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z)) {
		return true;
	}

	return false;
}

bool IsCollision(const AABB& aabb, const Sphere& sphere)
{
	Vector3 crossPoint;

	crossPoint.x = std::clamp(sphere.center.x, aabb.min.y, aabb.max.z);
	crossPoint.y = std::clamp(sphere.center.x, aabb.min.y, aabb.max.z);
	crossPoint.z = std::clamp(sphere.center.x, aabb.min.y, aabb.max.z);
	
	float distance = Length(Subtract(crossPoint, sphere.center));

	if (distance <= sphere.radius) {
		return true;
	}

	return false;
}

bool IsCollision(const AABB& aabb, const Segment& segment)
{
	float tMinX = (aabb.min.x - segment.origin.x) / segment.diff.x;
	float tMaxX = (aabb.max.x - segment.origin.x) / segment.diff.x;

	float tMinY = (aabb.min.y - segment.origin.y) / segment.diff.y;
	float tMaxY = (aabb.max.y - segment.origin.y) / segment.diff.y;

	float tMinZ = (aabb.min.z - segment.origin.z) / segment.diff.z;
	float tMaxZ = (aabb.max.z - segment.origin.z) / segment.diff.z;

	float tNearX = min(tMinX, tMaxX);
	float tFarX = max(tMinX, tMaxX);

	float tNearY = min(tMinY, tMaxY);
	float tFarY = max(tMinY, tMaxY);

	float tNearZ = min(tMinZ, tMaxZ);
	float tFarZ = max(tMinZ, tMaxZ);


	float tMin = max(max(tNearX, tNearY), tNearZ);
	float tMax = min(min(tFarX, tFarY), tFarZ);

	if (tMin <= tMax &&

		((tMin >= 0.0f && tMin <= 1.0f) || (tMax >= 0.0f && tMax <= 1.0f))) {

		return true;

	}

	return false;

}

