#pragma once
#include"Matrix4x4.h"
#include"Vector3.h"


struct Sphere {
	Vector3 center;
	float radius;
};

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjection, const Matrix4x4& viewPortMatrix, uint32_t color);


