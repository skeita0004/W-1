#pragma once

#pragma once

#include <vector>
#include <string>
#include "Sprite.hpp"
#include "Transform.hpp"

//-----------------------------------------------------------
//2D画像を管理する
//-----------------------------------------------------------
namespace Image
{
	//画像情報
	struct ImageData
	{
		//ファイル名
		std::string fileName;

		//ロードした画像データのアドレス
		Sprite*		pSprite;

		//切り抜き範囲
		RECT		rect;

		//アルファ
		float		alpha;

		//行列
		Transform transform;

		//コンストラクタ
		ImageData() : pSprite(nullptr)
		{
			fileName = "";
			alpha = 1.0f;
			pSprite = nullptr;
		}
	};


	//初期化
	void Initialize();

	//画像をロード
	//すでに同じ名前のファイルをロード済みの場合は、既存のデータの番号を返す
	//引数：fileName　ファイル名
	//戻値：その画像データに割り当てられた番号
	int Load(std::string fileName);

	//描画
	//引数：handle	描画したい画像の番号
	//引数：matrix	ワールド行列
	void Draw(int handle);

	//任意の画像を開放
	//引数：handle	開放したいモデルの番号
	void Release(int handle);

	//全ての画像を解放
	//（シーンが切り替わるときは必ず実行）
	void AllRelease();

	//切り抜き範囲の設定
	//引数：handle	設定したい画像の番号
	//引数：x		切り抜きたい範囲の左端Ｘ座標
	//引数：y		切り抜きたい範囲の上端Ｙ座標
	//引数：width	切り抜きたい範囲の幅
	//引数：height	切り抜きたい範囲の高さ
	void SetRect(int handle, int x, int y, int width, int height);

	//切り抜き範囲をリセット（画像全体を表示する）
	//引数：handle	設定したい画像の番号
	void ResetRect(int handle);

	//アルファ値を指定
	//引数：handle	設定したい画像の番号
	//引数：alpha アルファ値（不透明度）　0～255で指定
	void SetAlpha(int handle, int alpha);

	//ワールド行列を設定
	//引数：handle	設定したい画像の番号
	//引数：matrix	ワールド行列
	void SetTransform(int handle, Transform& transform);

	//ワールド行列の取得
	//引数：handle	知りたい画像の番号
	//戻値：ワールド行列
	XMMATRIX GetMatrix(int handle);
}