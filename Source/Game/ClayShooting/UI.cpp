// Engine
#include "../Engine/Input.hpp"
#include "../Engine/Image.hpp"
#include "../Engine/Audio.hpp"
#include "../Engine/SceneManager.hpp"
#include "../Engine/CameraSet.hpp"

// Game
#include "UI.hpp"
#include "Clay.hpp"

// その他
#include <DirectXMath.h>
#include "../include/imgui.h"

using namespace DirectX;
using namespace Config;
using namespace std;

namespace
{
}

// コンストラクタ
UI::UI(GameObject* parent)
    : GameObject(parent, "Reticle"),
    position_  {{ 0.0f,0.0f,0.0f }}, // 位置
    rotate_    {{ 0.0f,0.0f,0.0f }}, // 回転
    scale_     {{ 0.0f,0.0f,0.0f }}, // 拡大率
    hImage_    {(-1)},   // 画像読み込み
    hSound_    {(-1)},   // サウンド読み込み
    Bullet_num {{999}},  // 弾の弾数
    Game_Timer (31.0f)   // 試合時間
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
    hImage_[Bullet_UI1p && Bullet_UI2p]
        = Image::Load("Image/Bullet.png"); // 弾
    hImage_[Board_1p && Board_2p]
        = Image::Load("Image/UI_1.png");   // 木材

    assert(hImage_[Bullet_UI1p && Bullet_UI2p] >= 0);
    assert(hImage_[Board_1p && Board_2p] >= 0);

    // サウンドデータのロード
    hSound_
        = Audio::Load("Audio/拳銃を撃つ.wav"); // 弾
    assert(hSound_ >= 0);

    // 一応あると楽だからここに
    position_[Bullet_UI1p] = { -0.92f, -0.75f, 0.0f }; // 位置
    rotate_[Bullet_UI1p]   = {   0.0f,   0.0f, 0.0f }; // 回転
    scale_[Bullet_UI1p]    = {   0.3f,   0.3f, 0.0f }; // 拡大率

    position_[Bullet_UI2p] = {  0.63f, -0.75f,  0.0f }; // 位置
    rotate_[Bullet_UI2p]   = {  0.0f,    0.0f,  0.0f }; // 回転
    scale_[Bullet_UI2p]    = {  0.3f,    0.3f, 0.00f }; // 拡大率

    position_[Board_1p] = { -0.6f,  0.85f,  0.0f }; // 位置
    rotate_[Board_1p]   = {  0.0f,   0.0f,  0.0f }; // 回転
    scale_[Board_1p]    = {  0.7f,   0.4f, 0.00f }; // 拡大率

    position_[Board_2p] = { 0.6f,   0.85f,  0.0f }; // 位置
    rotate_[Board_2p]   = { 0.0f,    0.0f,  0.0f }; // 回転
    scale_[Board_2p]    = { 0.7f,    0.4f, 0.00f }; // 拡大率

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
        bool triggerNow[PLAYER_NUM]
            = { Input::GetPadTrrigerR(PLAYER_1) > 0.5f,
                Input::GetPadTrrigerR(PLAYER_2) > 0.5f };

        // 発砲するごとに弾を消費
        {
            if ((Input::IsMouseButtonDown(PLAYER_1)
                 || Input::IsPadButtonDown(XINPUT_GAMEPAD_B, PLAYER_1)
                 || triggerNow[PLAYER_1] && !triggerOld_[PLAYER_1]))
            {
                Audio::Play(hSound_);
                Bullet_num[PLAYER_1]--;
            }
            if ((Input::IsMouseButtonDown(PLAYER_2)
                 || Input::IsPadButtonDown(XINPUT_GAMEPAD_B, PLAYER_2)
                 || triggerNow[PLAYER_2] && !triggerOld_[PLAYER_2]))
            {
                Audio::Play(hSound_);
                Bullet_num[PLAYER_2]--;
            }
        }

        triggerOld_[PLAYER_1] = triggerNow[PLAYER_1];
        triggerOld_[PLAYER_2] = triggerNow[PLAYER_2];
    }
}

// 描画
void UI::Draw()
{
    // UI表示（弾）
    {
        // 1P
        {
            transform_.position_ = {position_[0].x,position_[0].y,position_[0].z};  // 位置
            transform_.scale_ = {scale_[0].x,scale_[0].y,scale_[0].z};  // 拡大率
            Image::SetTransform(hImage_[0], transform_);
            Image::Draw(hImage_[0]);

            transform_.position_ = {position_[2].x,position_[2].y,position_[2].z};  // 位置
            transform_.scale_ = {scale_[2].x,scale_[2].y,scale_[2].z};  // 拡大率
            Image::SetTransform(hImage_[1], transform_);
            Image::Draw(hImage_[1]);

            // 弾の残数
            char str[64];
            sprintf_s(str, "x %d", Bullet_num[PLAYER_1]);
            // sprintf_s(str, "x ---");
            textB_num_.Draw(90, 600, str);
        }

        // 2P
        {
            transform_.position_ = {position_[1].x,position_[1].y,position_[1].z};  // 位置
            transform_.scale_ = {scale_[1].x,scale_[1].y,scale_[1].z};  // 拡大率
            Image::SetTransform(hImage_[0], transform_);
            Image::Draw(hImage_[0]);

            transform_.position_ = {position_[3].x,position_[3].y,position_[3].z};  // 位置
            transform_.scale_ = {scale_[3].x,scale_[3].y,scale_[3].z};  // 拡大率
            Image::SetTransform(hImage_[1], transform_);
            Image::Draw(hImage_[1]);

            // 弾の残数
            char str[64];
            sprintf_s(str, "x %d", Bullet_num[PLAYER_2]);
            // sprintf_s(str, "x ---");
            textB_num_.Draw(1080, 600, str);
        }
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
