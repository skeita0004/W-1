#include "OverScene.hpp"
#include "SceneManager.hpp"
#include "Input.hpp"
#include "Image.hpp"

OverScene::OverScene(GameObject* _pParent) :
    GameObject(_pParent, "OverScene"),
    pSceneManager_(nullptr)
{
}

void OverScene::Init()
{
}

void OverScene::Update()
{
    if (Input::IsKeyDown(DIK_SPACE) or Input::IsPadButtonDown(XINPUT_GAMEPAD_B))
    {
        pSceneManager_->ChangeScene(SceneManager::SceneID::TITLE);
    }
}

void OverScene::Draw()
{
}

void OverScene::Release()
{
}
