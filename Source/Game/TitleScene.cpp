#include "TitleScene.hpp"
#include "SceneManager.hpp"
#include "Input.hpp"

TitleScene::TitleScene(GameObject* _pParent) :
    GameObject(_pParent, "TitleScene"),
    pSceneManager_(nullptr),
    hImage_(-1),
    hStartImage_(-1)
{
}

void TitleScene::Init()
{
}

void TitleScene::Update()
{
    if (Input::IsKeyDown(DIK_SPACE) or Input::IsPadButtonDown(XINPUT_GAMEPAD_B))
    {
        pSceneManager_->ChangeScene(SceneManager::SceneID::PLAY);
    }
}

void TitleScene::Draw()
{
}

void TitleScene::Release()
{
}
