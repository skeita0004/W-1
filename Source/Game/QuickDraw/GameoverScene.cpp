#include "GameoverScene.h"
#include "Engine/Image.h"

//コンストラクタ
GameoverScene::GameoverScene(GameObject * parent)
	: GameObject(parent, "GameoverScene"), hPict_(-1)
{
}

//初期化
void GameoverScene::Initialize()
{
	//画像データのロード
	hPict_ = Image::Load("Picture/GameoverScreen.jpg");
	assert(hPict_ >= 0);
	Image::SetRect(hPict_, 0, 0, 1280, 720);
}

//更新
void GameoverScene::Update()
{
}

//描画
void GameoverScene::Draw()
{
	Image::SetTransform(hPict_, transform_);
	Image::Draw(hPict_);
}

//開放
void GameoverScene::Release()
{
}