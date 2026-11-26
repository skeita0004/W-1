// Engine
#include "Input.hpp"
#include "Image.hpp"
#include "Audio.hpp"
#include "SceneManager.hpp"

// Game
#include "Reticle.hpp"
#include "Clay.hpp"
#include "Gun.hpp"
#include "UI.hpp"

// その他
#include <DirectXMath.h>
#include "imgui.h"

using namespace DirectX;
using namespace Config;
using namespace std;

namespace
{
    const int str_num = 64;

    // レティクルの初期値
    const XMFLOAT3 Init_Pos[PLAYER_NUM] // 位置
        = {{-0.3f,0.0f,0.0f}, {0.3f,0.0f,0.0f}};
    const XMFLOAT3 Init_Rot[PLAYER_NUM] // 回転
        = {{0.0f,0.0f,0.0f}, {0.0f,0.0f,0.0f}};
    const XMFLOAT3 Init_Sca[PLAYER_NUM] // 拡大率
        = {{0.2f,0.2f,0.0f}, {0.2f,0.2f,0.0f}};

    // レティクルの移動速度
    const float Speed = 0.02f;

    // 移動範囲
    const float Limit_X_Left  =  -0.9f; // 画面左端に行くと止まる
    const float Limit_X_Right =   0.9f; // 画面右端に行くと止まる
    const float Limit_Y_Up    =  0.82f; // 画面上側に行くと止まる
    const float Limit_Y_Down  =  -0.5f; // 画面下側に行くと止まる
}


// コンストラクタ
Reticle::Reticle(GameObject* parent)
    :GameObject(parent, "Reticle"),
    position_  {{ 0.0f,0.0f,0.0f }}, // 位置
    rotate_    {{ 0.0f,0.0f,0.0f }}, // 回転
    scale_     {{ 0.0f,0.0f,0.0f }}, // 拡大率
    hImage_{(-1)},   // 画像読み込み
    hSound_{(-1)}    // サウンド読み込み
{
}

// デストラクタ
Reticle::~Reticle()
{
}

// 初期化
void Reticle::Initialize()
{
    // レティクルの画像のロード
    {
        hImage_[PLAYER_1]
            = Image::Load("../Image/ClayShooting/Reticle_1P.png"); // レティクル(1P用)
        hImage_[PLAYER_2]
            = Image::Load("../Image/ClayShooting/Reticle_2P.png"); // レティクル(2P用)

        assert(hImage_[PLAYER_1] >= 0);
        assert(hImage_[PLAYER_2] >= 0);
    }

    // サウンドデータのロード
    {
        hSound_
            = Audio::Load("../Audio/ClayShooting/拳銃を撃つ.wav"); // 発砲音

        assert(hSound_ >= 0);
    }

    // 一応あると楽だからここに
    for (int i = 0; i < PLAYER_NUM; i++)
    {
        position_[i] = Init_Pos[i];
        rotate_[i]   = Init_Rot[i];
        scale_[i]    = Init_Sca[i];
    }

    textReticle_.Initialize("../Fonts/Rye-Regular.ttf", 28.0f);
}

// 更新
void Reticle::Update()
{
    // 標準移動
    Move();

    // 二人分
    for (int i = 0; i < PLAYER_NUM; i++) {
        // 発射
        Shoot(i);

        // 画面を超えないように指定
        if (position_[i].x <= Limit_X_Left)  position_[i].x = Limit_X_Left;  // 左
        if (position_[i].x >= Limit_X_Right) position_[i].x = Limit_X_Right; // 右
        if (position_[i].y >= Limit_Y_Up)    position_[i].y = Limit_Y_Up;    // 上
        if (position_[i].y <= Limit_Y_Down)  position_[i].y = Limit_Y_Down;  // 下
    }
}

// 描画
void Reticle::Draw()
{
    //transform_.position = {Init_Pos[PLAYER_NUM]};  // 位置
    //transform_.scale    = {Init_Sca[PLAYER_NUM]};  // 拡大率

    // レティクル表示
    for (int i = 0; i < PLAYER_NUM; i++) {
        Image::SetTransform(hImage_[i], transform_);
        Image::Draw(hImage_[i]);
    }

    // ImGui (デバック用)
    {
        /*
        // ======= レティクルの位置座標 =======
        ImGui::Begin("Reticle_position");
        ImGui::InputFloat("pos1P.X", &position_[0].x);
        ImGui::InputFloat("pos1P.Y", &position_[0].y);
        ImGui::InputFloat("pos2P.X", &position_[1].x);
        ImGui::InputFloat("pos2P.Y", &position_[1].y);
        //ImGui::InputFloat("pos.Z", &position_.z);
        ImGui::End();
        // ======= 発射の状況 =======
        // 右トリガーを一瞬だけ押したときに使う、メンバ変数
        bool triggerNow[2] = { Input::GetPadTrrigerR(0) > 0.5f, Input::GetPadTrrigerR(1) > 0.5f };

        ImGui::Begin("Shoot_status");
        if ((Input::IsMouseButtonDown(0)
            || Input::IsPadButtonDown(XINPUT_GAMEPAD_B, 0)
            || triggerNow[0] && !triggerOld_[0]))
        {
            ImGui::Text("Shoot!!");
            ImGui::Text("%.2f", position_[0].x);
            ImGui::Text("%.2f", position_[0].y);
        }
        if ((Input::IsMouseButtonDown(1)
            || Input::IsPadButtonDown(XINPUT_GAMEPAD_B, 1)
            || triggerNow[1] && !triggerOld_[1]))
        {
            ImGui::Text("Shoot!!");
            ImGui::Text("%.2f", position_[1].x);
            ImGui::Text("%.2f", position_[1].y);
        }
        ImGui::End();

        triggerOld_[0] = triggerNow[0];
        triggerOld_[1] = triggerNow[1];
        */
    }
}

// 開放
void Reticle::Release()
{
}

// 標準移動
void Reticle::Move()
{
    // コントローラー対応
    for (int i = 0; i < PLAYER_NUM; i++)
    {
        //移動ベクトル
        XMFLOAT3 moveVector = Input::GetPadStickL(i);

        // XMFLOAT3 から float への変換が必要
        position_[i].x += moveVector.x * Speed;
        position_[i].y += moveVector.y * Speed;
    }

    // 1P
    if (Input::IsKey(DIK_W)) position_[PLAYER_1].y += Speed;  // 上
    if (Input::IsKey(DIK_S)) position_[PLAYER_1].y -= Speed;  // 下
    if (Input::IsKey(DIK_D)) position_[PLAYER_1].x += Speed;  // 右
    if (Input::IsKey(DIK_A)) position_[PLAYER_1].x -= Speed;  // 左

    // 2P ※デバッグ用
    if (Input::IsKey(DIK_I)) position_[PLAYER_2].y += Speed;  // 上
    if (Input::IsKey(DIK_K)) position_[PLAYER_2].y -= Speed;  // 下
    if (Input::IsKey(DIK_L)) position_[PLAYER_2].x += Speed;  // 右
    if (Input::IsKey(DIK_J)) position_[PLAYER_2].x -= Speed;  // 左
}

// 発射
void Reticle::Shoot(int player)
{
    if (player < 0 || player >= PLAYER_NUM) return;

    // 右トリガーを一瞬だけ押したときに使う、メンバ変数
    bool triggerNow = Input::GetPadTrrigerR(player) > 0.5f;

    if (Input::IsMouseButtonDown(player)
        || Input::IsPadButtonDown(XINPUT_GAMEPAD_B, player)
        || triggerNow && !triggerOld_[player])
    {
        // 発射
        Audio::Play(hSound_);

        // クレーとの当たり判定
        Clay* pClay = static_cast<Clay*>(FindChildObject("Clay"));
        if (pClay != nullptr) {
            pClay->TryBreak(player,XMFLOAT2(position_[player].x, position_[player].y));
        }

        //弾をシーンに追加
        //GameObject* playScene = FindObject("PlayScene");
        //Bullet* pBullet = Instantiate<Bullet>(FindObject("PlayScene"));

        //発射方向 = 先端 - 根本
        //XMFLOAT3 dir;
        //XMStoreFloat3(&dir, XMLoadFloat3(&position_) - XMLoadFloat3(&rotate_));

        //砲台の先端と根本の位置を取得
        //XMMATRIX shotPos = Image::GetMatrix(hImage_);    //先端
        //XMMATRIX cannonRoot = Image::GetMatrix(hImage_); //根本
        // position_.x;

        //撃つ
        //pBullet->Shot(position_, dir);
    }

    triggerOld_[player] = triggerNow;
}
