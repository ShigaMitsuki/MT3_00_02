#include "Polygon.h"
#include<Novice.h>
#define _USE_MATH_DEFINES
#include<Math.h>

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