#include "UI.h"
#include "Engine/Input.h"
#include "Engine/Image.h"
#include "Engine/Audio.h"
#include "Engine/SceneManager.h"
#include "Clay.h"

#include <DirectXMath.h>

#include "../include/imgui.h"

using namespace DirectX;
using namespace std;

// コンストラクタ
UI::UI(GameObject* parent)
    : GameObject(parent, "Reticle"),
    position_{ { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f },{ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f } },
    rotate_{ { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f },{ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f } },
    scale_{ { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f },{ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f } },
    hImage_{ -1, -1 }, hSound_(-1),
    Bullet_num{ 999, 999 }, /*Score(0),*/ Game_Timer(13.0f)
{
}

// デストラクタ
UI::~UI()
{
}

// 初期化
void UI::Initialize()
{
    // UIの画像のロード
    hImage_[0] = Image::Load("Image/Bullet.png"); // 弾
    assert(hImage_[0] >= 0);
    hImage_[1] = Image::Load("Image/UI_1.png");   // 木材
    assert(hImage_[1] >= 0);

    // サウンドデータのロード
    hSound_ = Audio::Load("Audio/拳銃を撃つ.wav"); // 弾
    assert(hSound_ >= 0);

    // 一応あると楽だからここに
    position_[0] = { -0.92f, -0.75f, 0.0f }; // 位置
    rotate_[0]   = {   0.0f,   0.0f, 0.0f }; // 回転
    scale_[0]    = {   0.3f,   0.3f, 0.0f }; // 拡大率

    position_[1] = {  0.63f, -0.75f,  0.0f }; // 位置
    rotate_[1]   = {  0.0f,    0.0f,  0.0f }; // 回転
    scale_[1]    = {  0.3f,    0.3f, 0.00f }; // 拡大率

    position_[2] = { -0.6f,  0.85f,  0.0f }; // 位置
    rotate_[2]   = {  0.0f,   0.0f,  0.0f }; // 回転
    scale_[2]    = {  0.7f,   0.4f, 0.00f }; // 拡大率

    position_[3] = { 0.6f,   0.85f,  0.0f }; // 位置
    rotate_[3]   = { 0.0f,    0.0f,  0.0f }; // 回転
    scale_[3]    = { 0.7f,    0.4f, 0.00f }; // 拡大率

    textB_num_.Initialize("Fonts/Rye-Regular.ttf", 64.0f);
    textScore_.Initialize("Fonts/Rye-Regular.ttf", 64.0f);
    textTimer_.Initialize("Fonts/Rye-Regular.ttf", 86.0f);
}

// 更新
void UI::Update()
{
    // 時間仮置き
    if (Game_Timer >= 0.0f)
    {
        Game_Timer -= 0.02f;

        //敵がもういなかったらクリア画面へ
        if (Game_Timer <= 0.0f) {
            SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
            pSceneManager->ChangeScene(SCENE_ID_CLEAR);
        }
    }

    // 発砲ごとに残弾数消費
    {
        bool triggerNow[2] = { Input::GetPadTrrigerR(0) > 0.5f, Input::GetPadTrrigerR(1) > 0.5f };

        if ((Input::IsMouseButtonDown(0)
            || Input::IsPadButtonDown(XINPUT_GAMEPAD_B, 0)
            || triggerNow[0] && !triggerOld_[0])) {
            Audio::Play(hSound_);
            Bullet_num[0]--;
            //Score += 20;
        }
        if ((Input::IsMouseButtonDown(1)
            || Input::IsPadButtonDown(XINPUT_GAMEPAD_B, 1)
            || triggerNow[1] && !triggerOld_[1])) {
            Audio::Play(hSound_);
            Bullet_num[1]--;
        }

        triggerOld_[0] = triggerNow[0];
        triggerOld_[1] = triggerNow[1];
    }
}

// 描画
void UI::Draw()
{
    // UI表示（弾）
    {
        // 1P
        transform_.position_ = { position_[0].x,position_[0].y,position_[0].z };  // 位置
        transform_.scale_ = { scale_[0].x,scale_[0].y,scale_[0].z };  // 拡大率
        Image::SetTransform(hImage_[0], transform_);
        Image::Draw(hImage_[0]);

        transform_.position_ = { position_[2].x,position_[2].y,position_[2].z };  // 位置
        transform_.scale_ = { scale_[2].x,scale_[2].y,scale_[2].z };  // 拡大率
        Image::SetTransform(hImage_[1], transform_);
        Image::Draw(hImage_[1]);

        // 2P
        transform_.position_ = { position_[1].x,position_[1].y,position_[1].z };  // 位置
        transform_.scale_ = { scale_[1].x,scale_[1].y,scale_[1].z };  // 拡大率
        Image::SetTransform(hImage_[0], transform_);
        Image::Draw(hImage_[0]);

        transform_.position_ = { position_[3].x,position_[3].y,position_[3].z };  // 位置
        transform_.scale_ = { scale_[3].x,scale_[3].y,scale_[3].z };  // 拡大率
        Image::SetTransform(hImage_[1], transform_);
        Image::Draw(hImage_[1]);

        // 弾の残数
        char str[64];
        sprintf_s(str, "x %d", Bullet_num[0]);
        // sprintf_s(str, "x ---");
        textB_num_.Draw(90, 600, str);

        // 弾の残数
        sprintf_s(str, "x %d", Bullet_num[1]);
        // sprintf_s(str, "x ---");
        textB_num_.Draw(1080, 600, str);
    }

    // タイマー表示
    {
        char str[64];
        sprintf_s(str, "%.1f", Game_Timer);
        if (Game_Timer > 10.0f) {
            textTimer_.Draw(540, 10, str);
        }
        else {
            textTimer_.Draw(570, 10, str);
        }
        sprintf_s(str, "s");
        textTimer_.Draw(720, 10, str);
    }

    // スコア表示
    {
        char str[64];
        sprintf_s(str, "1P :%.5d", sc->GetTotalScore_1P());
        textScore_.Draw(120, 20, str);

        //char str[64];
        sprintf_s(str, "2P :%.5d", sc->GetTotalScore_2P());
        textScore_.Draw(880, 20, str);
    }

    // レティクル表示
    //transform_.position_ = { position_[1].x,position_[1].y,position_[1].z };  // 位置
    //transform_.scale_ = { scale_[1].x,scale_[1].y,scale_[1].z };  // 拡大率
    //Image::SetTransform(hImage_[1], transform_);
    //Image::Draw(hImage_[1]);

    // ImGui (デバック用)
    {
        /*
        // ======= UIの位置座標 =======
        ImGui::Begin("UI_position");
        ImGui::InputFloat("pos.X1", &position_[0].x);
        ImGui::InputFloat("pos.Y1", &position_[0].y);
        //ImGui::InputFloat("pos.Z1", &position_[0].z);
        ImGui::InputFloat("pos.X2", &position_[1].x);
        ImGui::InputFloat("pos.Y2", &position_[1].y);
        //ImGui::InputFloat("pos.Z2", &position_[1].z);
        ImGui::End();
        */
    }
}

// 開放
void UI::Release()
{
}
