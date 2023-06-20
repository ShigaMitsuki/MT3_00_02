#pragma once
#include"Matrix4x4.h"
#include"Vector3.h"


struct Sphere {
	Vector3 center;
	float radius;
};

struct Plane {
	Vector3 normal;
	float distance;
};

struct Triangle {
	Vector3 vertices[3];
};

//struct Segment {
//	Vector3 origin;
//	Vector3 diff;
//};

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjection, const Matrix4x4& viewPortMatrix, uint32_t color);
void DrawPlane(const Plane& plane,const Matrix4x4& viewProjection, const Matrix4x4& viewPortMatrix ,uint32_t color);
void DrawLine(const Segment& segment, const Matrix4x4& viewProjection, const Matrix4x4& viewPortMatrix, uint32_t color);
void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjection, const Matrix4x4& viewPortMatrix, uint32_t color);

bool IsCollision(const Sphere& SphereA, const Sphere& SphereB);
bool IsCollision(const Sphere& Sphere, const Plane& plane);
bool IsCollision(const Segment& segment, const Plane& plane);

bool IsCollision(const Segment& segment, const Triangle& triangle);