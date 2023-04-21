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

	Matrix4x4 m1 = {
		3.2f,0.7f,9.6f,4.4f,
		5.5f,1.3f,7.8f,2.1f,
		6.9f,8.0f,2.6f,1.0f,
		0.5f,7.2f,5.1f,3.3f
	};

	Matrix4x4 m2 = {
		4.1f,6.5f,3.3f,2.2f,
		8.8f,0.6f,9.9f,7.7f,
		1.1f,5.5f,6.6f,0.0f,
		3.3f,9.9f,8.8f,2.2f
	};

	Matrix4x4 ResultAdd = Add(m1, m2);
	Matrix4x4 ResultMultiply = Multiply(m1, m2);
	Matrix4x4 ResultSubtract = Subtract(m1, m2);
	Matrix4x4 InverseM1 = Inverse(m1);
	Matrix4x4 InverseM2 = Inverse(m2);
	Matrix4x4 TransposeM1 = Transpose(m1);
	Matrix4x4 TransposeM2 = Transpose(m2);
	Matrix4x4 Identity = MakeIdentity4x4();

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

		MatrixScreenPrintf(0,0 ,ResultAdd , "Add");
		MatrixScreenPrintf(0,kRowHeight * 5 , ResultSubtract, "Subtract");
		MatrixScreenPrintf(0, kRowHeight * 5 * 2, ResultMultiply, "Multiply");
		MatrixScreenPrintf(0, kRowHeight * 5 * 3,InverseM1 , "InverseM1");
		MatrixScreenPrintf(0, kRowHeight * 5 * 4, InverseM2, "InverseM2");
		MatrixScreenPrintf(kColmnWidth * 5, 0, TransposeM1, "TransposeM1");
		MatrixScreenPrintf(kColmnWidth * 5, kRowHeight * 5, TransposeM2, "TransposeM2");
		MatrixScreenPrintf(kColmnWidth * 5, kRowHeight * 5 * 2, Identity,"Identity");

		

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

