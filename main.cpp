#include <Novice.h>
#include <Math.h>
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix4x4.h"

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

	Vector3 kLocalVertices[3];

	kLocalVertices[0] = { -0.5f,-0.5f,0.0f };
	kLocalVertices[1] = { 0.5f,-0.5f,0.0f };
	kLocalVertices[2] = { 0.0f,0.5f,0.0f };

	Vector3 Rotate{};
	Vector3 Translate{};
	Vector3 CameraPosition{0,0,-10};

	Matrix4x4 WorldMatrix = MakeAffineMatrix({1.0f,1.0f,1.0f},Rotate,Translate);
	Matrix4x4 CameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f },{0.0f,0.0f ,0.0f }, CameraPosition);
	Matrix4x4 ViewMatrix = Inverse(CameraMatrix);
	Matrix4x4 ProjectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindwWidth) / float(kWindwHeight),0.1f,100.0f);
	Matrix4x4 WorldViewProjectionMatrix = Multiply(WorldMatrix, Multiply(ViewMatrix, ProjectionMatrix));
	Matrix4x4 ViewPortMatrix = MakeViewportMatrix(0,0,float(kWindwWidth),float(kWindwHeight),0.0f,1.0f);
	
	Vector3 ScreenVertices[3];
	for (uint32_t i = 0; i < 3; ++i) {
		Vector3 ndcVertex = Transform(kLocalVertices[i], WorldViewProjectionMatrix);
		ScreenVertices[i] = Transform(ndcVertex, ViewPortMatrix);
	}

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


		if (keys[DIK_W] != 0) {
				Translate.z += 0.1f;
			
		}
		if (keys[DIK_S] != 0) {
				Translate.z -= 0.1f;
			
		}
		if (keys[DIK_D] != 0) {
			
				Translate.x += 0.1f;
			
		}
		if (keys[DIK_A] != 0) {
				Translate.x -= 0.1f;
		}

		Rotate.y += 0.1f;

		if (Rotate.y >= 360) {
			Rotate.y -= 360;
		}

		WorldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, Rotate, Translate);
		CameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f ,0.0f }, CameraPosition);
		ViewMatrix = Inverse(CameraMatrix);
		ProjectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindwWidth) / float(kWindwHeight), 0.1f, 100.0f);
		WorldViewProjectionMatrix = Multiply(WorldMatrix, Multiply(ViewMatrix, ProjectionMatrix));
		ViewPortMatrix = MakeViewportMatrix(0, 0, float(kWindwWidth), float(kWindwHeight), 0.0f, 1.0f);

		//ScreenVertices[3];
		for (uint32_t i = 0; i < 3; ++i) {
			Vector3 ndcVertex = Transform(kLocalVertices[i], WorldViewProjectionMatrix);
			ScreenVertices[i] = Transform(ndcVertex, ViewPortMatrix);
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		VectorScreenPrintf(0, 100, cross, "Cross");

		for (int i = 0; i < 3; i++) {

			VectorScreenPrintf(0, 20 * i, ScreenVertices[i], "vertices");
		}

		Novice::DrawTriangle(
			int(ScreenVertices[0].x), int(ScreenVertices[0].y),
			int(ScreenVertices[1].x), int(ScreenVertices[1].y),
			int(ScreenVertices[2].x), int(ScreenVertices[2].y),
			RED, kFillModeSolid
		);

		/*Novice::DrawTriangle(
			int(kLocalVertices[0].x), int(kLocalVertices[0].y),
			int(kLocalVertices[1].x), int(kLocalVertices[1].y),
			int(kLocalVertices[2].x), int(kLocalVertices[2].y),
			RED, kFillModeSolid
		);*/

		VectorScreenPrintf(400, 0, Rotate, "Rotate");
		VectorScreenPrintf(400, 20, Translate, "Translate");
		VectorScreenPrintf(400, 40, CameraPosition, "CameraPosition");
		


		

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

