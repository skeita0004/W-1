#include "ClearScene.h"
#include "../Engine/Image.h"
#include "../Engine/SceneManager.h"

//コンストラクタ
ClearScene::ClearScene(GameObject * parent)
	: GameObject(parent, "ClearScene"), hPict_(-1)
{
}

//初期化
void ClearScene::Initialize()
{
	//画像データのロード
	hPict_ = Image::Load("Picture/ClearScreen.jpg");
	assert(hPict_ >= 0);
	Image::SetRect(hPict_, 0, 0, 1280, 720);
}

//更新
void ClearScene::Update()
{
	static float dt;
	dt += deltaTime;
	if(dt >= 3.0f)
	{
		// シーンを切り替える
		SceneManager* sceneManager = (SceneManager*)GetParent();
		sceneManager->ChangeScene(SCENE_ID_TITLE);
	}
}

//描画
void ClearScene::Draw()
{
		Image::SetTransform(hPict_, transform_);
		Image::Draw(hPict_);
}

//開放
void ClearScene::Release()
{
}