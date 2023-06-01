#include <Novice.h>
#include <Math.h>
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix4x4.h"
#include "Polygon.h"
#include<imgui.h>

const char kWindowTitle[] = "LD2A_07_シガ_ミツキ";

static const int kColmnWidth = 100;
static const int kRowHeight = 20;
void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label) {
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColmnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColmnWidth * 2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x + kColmnWidth * 3, y, "%s", label);
}

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label) {
	Novice::ScreenPrintf(x,y, "%s", label);
	for (int row = 1; row < 5; ++row) {
		for (int column = 0; column < 4; ++column) {
			Novice::ScreenPrintf(x + column * kColmnWidth, y + row * kRowHeight, "%6.02f", matrix.m[row - 1][column]);
		}
	}
}

const int kWindwWidth = 1280;
const int kWindwHeight = 720;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);
	
	Vector3 v1{1.2f,-3.9f,2.5f };
	Vector3 v2{ 2.8f,0.4f,-1.3f};
	Vector3 cross = Cross(v1, v2);
	
	Vector3 Translate{};
	Vector3 Rotate{};

	Vector3 CameraTranslate{ 0.0f,1.9f,-6.49f };
	Vector3 CameraRotate{0.26f,0.0f,0.0f};
	//Vector3 CameraPosition{};

	Segment segment{ {-2.0f,-1.0f,0.0f} ,{3.0f,2.0f,2.0f} };
	Vector3 point{ -1.5f,0.0f,0.6f };

	Vector3 project = Project(Subtract(point, segment.origin), segment.diff);
	Vector3 clossPoint = ClosestPoint(point, segment);

	Sphere pointSphere = { point , 0.01f };
	Sphere clossPointSphere = { clossPoint, 0.01f };

	Matrix4x4 WorldMatrix = MakeAffineMatrix({1.0f,1.0f,1.0f},Rotate,Translate);
	Matrix4x4 CameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, CameraRotate, CameraTranslate);
	Matrix4x4 ViewMatrix = Inverse(CameraMatrix);
	Matrix4x4 ProjectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindwWidth) / float(kWindwHeight),0.1f,100.0f);
	Matrix4x4 WorldViewProjectionMatrix = Multiply(WorldMatrix, Multiply(ViewMatrix, ProjectionMatrix));
	Matrix4x4 ViewPortMatrix = MakeViewportMatrix(0,0,float(kWindwWidth),float(kWindwHeight),0.0f,1.0f);
	
	Vector3 start = Transform(Transform(segment.origin, WorldViewProjectionMatrix), ViewPortMatrix);
	Vector3 end = Transform(Transform(Add(segment.origin, segment.diff), WorldViewProjectionMatrix), ViewPortMatrix);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		if (keys[DIK_O] != 0) {
			//CameraPosition.x += 0.5f;
		}

		project = Project(Subtract(point, segment.origin), segment.diff);
		clossPoint = ClosestPoint(point, segment);

		pointSphere = { point , 0.01f };
		clossPointSphere = { clossPoint, 0.01f };

		WorldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, Rotate, Translate);
		CameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, CameraRotate, CameraTranslate);
		ViewMatrix = Inverse(CameraMatrix);
		ProjectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindwWidth) / float(kWindwHeight), 0.1f, 100.0f);
		WorldViewProjectionMatrix = Multiply(WorldMatrix, Multiply(ViewMatrix, ProjectionMatrix));
		ViewPortMatrix = MakeViewportMatrix(0, 0, float(kWindwWidth), float(kWindwHeight), 0.0f, 1.0f);

		start = Transform(Transform(segment.origin, WorldViewProjectionMatrix), ViewPortMatrix);
		end = Transform(Transform(Add(segment.origin, segment.diff), WorldViewProjectionMatrix), ViewPortMatrix);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		ImGui::Begin("Window");
		//ImGui::DragFloat3("CameraTranslate", &CameraTranslate.x, 0.01f);
		//ImGui::DragFloat3("CameraRotate", &CameraRotate.x, 0.01f);/*
		//
		//ImGui::DragFloat("SphereRadius", &sphere.radius, 0.01f);*/

		ImGui::DragFloat3("point", &point.x, 0.01f);

		ImGui::InputFloat3("Project", &project.x, " %.3f",ImGuiInputTextFlags_ReadOnly);
		ImGui::End();

		DrawGrid(WorldViewProjectionMatrix,ViewPortMatrix);
		DrawSphere(pointSphere, WorldViewProjectionMatrix, ViewPortMatrix, RED);;
		DrawSphere(clossPointSphere, WorldViewProjectionMatrix, ViewPortMatrix, BLACK);;

		Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), WHITE);

		Novice::ScreenPrintf(0, 0, "%0.1f,%0.1f,%0.1f,%0.1f", start.x, start.y, end.x, end.y);

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

