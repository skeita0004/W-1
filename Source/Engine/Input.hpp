#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dInput.h>
#include <XInput.h>
#include <DirectXMath.h>

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dInput8.lib")
#pragma comment(lib, "Xinput.lib")


using namespace DirectX;

//-----------------------------------------------------------
//DirectInputを使ったキーボード入力処理
//-----------------------------------------------------------
namespace Input
{
    enum MOUSE : int
    {
        LEFT = 0,
        RIGHT,
        MIDDLE,
    };

	//初期化
	//引数：hWnd	ウィンドウハンドル
	void Initialize(HWND hWnd);

	//更新
	void Update();


	///////////////////////////　キーボード　//////////////////////////////////
	//キーが押されているか調べる
	//引数：keyCode	調べたいキーのコード
	//戻値：押されていればtrue
	bool IsKey(int keyCode);

	//キーを今押したか調べる（押しっぱなしは無効）
	//引数：keyCode	調べたいキーのコード
	//戻値：押した瞬間だったらtrue
	bool IsKeyDown(int keyCode);

	//キーを今放したか調べる
	//引数：keyCode	調べたいキーのコード
	//戻値：放した瞬間だったらtrue
	bool IsKeyUp(int keyCode);


	///////////////////////////　マウス　//////////////////////////////////
	//マウスのボタンが押されているか調べる
	//引数：buttonCode	調べたいボタンの番号
	//戻値：押されていればtrue
	bool IsMouseButton(int buttonCode);

	//マウスのボタンを今押したか調べる（押しっぱなしは無効）
	//引数：buttonCode	調べたいボタンの番号
	//戻値：押した瞬間だったらtrue
	bool IsMouseButtonDown(int buttonCode);

	//マウスのボタンを今放したか調べる
	//引数：buttonCode	調べたいボタンの番号
	//戻値：放した瞬間だったらtrue
	bool IsMouseButtonUp(int buttonCode);


	//マウスカーソルの位置を取得
	//戻値：マウスカーソルの位置
	XMFLOAT3 GetMousePosition();

	//マウスカーソルの位置をセット
	//引数：マウスカーソルの位置
	void SetMousePosition(int x, int y);

	//そのフレームでのマウスの移動量を取得
	//戻値：X,Y マウスの移動量　／　Z,ホイールの回転量
	XMFLOAT3 GetMouseMove();


	///////////////////////////　コントローラー　//////////////////////////////////
	//コントローラーのボタンが押されているか調べる
	//引数：buttonCode	調べたいボタンの番号
	//戻値：押されていればtrue
	bool IsPadButton(int buttonCode, int padID = 0);

	//コントローラーのボタンを今押したか調べる（押しっぱなしは無効）
	//引数：buttonCode	調べたいボタンの番号
	//戻値：押した瞬間だったらtrue
	bool IsPadButtonDown(int buttonCode, int padID = 0);

	//コントローラーのボタンを今放したか調べる
	//引数：buttonCode	調べたいボタンの番号
	//戻値：放した瞬間だったらtrue
	bool IsPadButtonUp(int buttonCode, int padID = 0);

	//左スティックの傾きを取得
	//戻値:傾き具合（-1～1）
	XMFLOAT3 GetPadStickL(float _sensitivity = 1.0f,
                          int padID = 0);

	//右スティックの傾きを取得
	//戻値:傾き具合（-1～1）
	XMFLOAT3 GetPadStickR(float _sensitivity = 1.0f,
                          int padID = 0);

	//左トリガーの押し込み具合を取得
	//戻値:押し込み具合（0～1）
	float		GetPadTrrigerL(int padID = 0);

	//右トリガーの押し込み具合を取得
	//戻値:押し込み具合（0～1）
	float		GetPadTrrigerR(int padID = 0);

	//振動させる
	void SetPadVibration(int l, int r, int padID = 0);

    //開放
	void Release();
};
