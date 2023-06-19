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
	
	Vector3 Translate{};
	Vector3 Rotate{};

	Vector3 CameraTranslate{ 0.0f,1.9f,-6.49f };
	Vector3 CameraRotate{0.26f,0.0f,0.0f};
	//Vector3 CameraPosition{};

	Segment segment = {
		{ 0.0f,0.0f,0.0f},
		{ 1.0f,1.0f,1.0f}
	};

	Plane plane = {
		{ 0.0f,1.0f,0.0f},
		0.5f
	};

	int color = WHITE;

	Matrix4x4 WorldMatrix = MakeAffineMatrix({1.0f,1.0f,1.0f},Rotate,Translate);
	Matrix4x4 CameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, CameraRotate, CameraTranslate);
	Matrix4x4 ViewMatrix = Inverse(CameraMatrix);
	Matrix4x4 ProjectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindwWidth) / float(kWindwHeight),0.1f,100.0f);
	Matrix4x4 WorldViewProjectionMatrix = Multiply(WorldMatrix, Multiply(ViewMatrix, ProjectionMatrix));
	Matrix4x4 ViewPortMatrix = MakeViewportMatrix(0,0,float(kWindwWidth),float(kWindwHeight),0.0f,1.0f);

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
		

		WorldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, Rotate, Translate);
		CameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, CameraRotate, CameraTranslate);
		ViewMatrix = Inverse(CameraMatrix);
		ProjectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindwWidth) / float(kWindwHeight), 0.1f, 100.0f);
		WorldViewProjectionMatrix = Multiply(WorldMatrix, Multiply(ViewMatrix, ProjectionMatrix));
		ViewPortMatrix = MakeViewportMatrix(0, 0, float(kWindwWidth), float(kWindwHeight), 0.0f, 1.0f);

		if (IsCollision(segment, plane)) {
			color = RED;
		}
		else {
			color = WHITE;
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		//sImGui::Begin("Window");
		//ImGui::DragFloat3("CameraTranslate", &CameraTranslate.x, 0.01f);
		//ImGui::DragFloat3("CameraRotate", &CameraRotate.x, 0.01f);/*
		//
		ImGui::DragFloat3("origin", &segment.origin.x, 0.01f);
		ImGui::DragFloat3("diff", &segment.diff.x, 0.01f);
		ImGui::DragFloat("distance", &plane.distance, 0.01f);
		ImGui::DragFloat3("normal", &plane.normal.x, 0.01f);

		plane.normal = Normalize(plane.normal);

		DrawGrid(WorldViewProjectionMatrix,ViewPortMatrix);
		DrawLine(segment, WorldViewProjectionMatrix, ViewPortMatrix, color);
		DrawPlane(plane, WorldViewProjectionMatrix, ViewPortMatrix, color);

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

