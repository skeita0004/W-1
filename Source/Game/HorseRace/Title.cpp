#include "Title.hpp"
#include "../Engine/Image.h"
#include "../Engine/Input.h"
#include  "../Engine/SceneManager.h"

//コンストラクタ
Title::Title(GameObject* parent)
	: GameObject(parent, "Title"), hImage_(-1)
{
}

//初期化
void Title::Initialize()
{
	//画像データのロード
	hImage_ = Image::Load("Picture/Title.png");
	assert(hImage_ >= 0);
	Image::SetRect(hImage_, 0, 0, 3500, 1500);
	transform_.scale_ = XMFLOAT3(0.5, 0.5, 0.5);
}

//更新
void Title::Update()
{
	if (Input::IsPadButtonDown(XINPUT_GAMEPAD_A) || Input::IsKeyDown(DIK_SPACE))
	{
		// シーンを切り替える
		SceneManager* sceneManager = (SceneManager*)GetParent();
		sceneManager->ChangeScene(SCENE_ID_TEST);
	}
}

//描画
void Title::Draw()
{
	Image::SetTransform(hImage_, transform_);
	Image::Draw(hImage_);
}

//開放
void Title::Release()
{
}