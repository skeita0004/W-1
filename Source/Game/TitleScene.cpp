#include "TitleScene.hpp"
#include "SceneManager.hpp"
#include "Input.hpp"

#include <imgui.h>
#include <XInput.h>
#include <cmath>

//コンストラクタ
TitleScene::TitleScene(GameObject* parent)
    : GameObject(parent, "TitleScene"),
    sceneManager_(static_cast<SceneManager*>(parent))
{
}

//初期化
void TitleScene::Initialize()
{
    titleText_.Initialize("Font/Rye-Regular.ttf", 64.0f);
    pushText_.Initialize("Font/Rye-Regular.ttf", 30.0f);
}

//更新
void TitleScene::Update()
{
    if (Input::IsKeyDown(DIK_RETURN) ||
        Input::IsPadButtonDown(XINPUT_GAMEPAD_A, 0) ||
        Input::IsPadButtonDown(XINPUT_GAMEPAD_A, 1))
    {
        sceneManager_->ChangeScene(SceneManager::SceneID::TEST);
    }
}

//描画
void TitleScene::Draw()
{
    const ImVec2 size = ImGui::GetIO().DisplaySize;

    titleText_.Draw(size.x * 0.5f - 220.0f, 80.0f, "Western-one");

    //ゆっくり点滅
    const float time = static_cast<float>(ImGui::GetTime());
    const float alpha = (sinf(time * 1.5f) + 1.0f) * 0.5f;

    const unsigned int color = IM_COL32(
        255, 255, 255, static_cast<int>(alpha * 255.0f));

    pushText_.Draw(
        size.x * 0.5f - 230.0f,
        size.y * 0.65f,
        "Push [Enter] or XBOX (A)",
        color);
}

//開放
void TitleScene::Release()
{
    titleText_.Release();
    pushText_.Release();
}