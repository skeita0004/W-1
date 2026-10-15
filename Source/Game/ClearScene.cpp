#include "ClearScene.hpp"
#include "SceneManager.hpp"
#include "Input.hpp"
#include "Image.hpp"

ClearScene::ClearScene(GameObject* _pParent) :
    GameObject(_pParent, "ClearScene"),
    pSceneManager_(nullptr)
{
}

void ClearScene::Init()
{
}

void ClearScene::Update()
{
    if (Input::IsKeyDown(DIK_SPACE) or Input::IsPadButtonDown(XINPUT_GAMEPAD_B))
    {
        pSceneManager_->ChangeScene(SceneManager::SceneID::TITLE);
    }
}

void ClearScene::Draw()
{
}

void ClearScene::Release()
{
}
