#include <Novice.h>
#include <Math.h>
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix4x4.h"

const char kWindowTitle[] = "LD2A_07_シガ_ミツキ";

static const int kColmnWidth = 60;
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

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);
	
	Matrix4x4 OrthoGraphicMatrix = MakeOrthoGraphicMatrix (-160.0f, 160.0f,200.0f,300.0f,0.0f,1000.0f);

	Matrix4x4 PerspectiveFovMatrix = MakePerspectiveFovMatrix(0.63f,1.33f,0.1f, 1000.0f);
	
	Matrix4x4 ViewportMatrix = MakeViewportMatrix(100.0f, 200.0f, 600.0f,300.0f,0.0f, 1.0f);
	
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

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		MatrixScreenPrintf(0, 0, OrthoGraphicMatrix, "OrthoGraphicMatrix");
		MatrixScreenPrintf(0, kRowHeight * 5, PerspectiveFovMatrix, "PerspectiveFovMatrix");
		MatrixScreenPrintf(0, kRowHeight * 10, ViewportMatrix, "ViewportMatrix");

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

