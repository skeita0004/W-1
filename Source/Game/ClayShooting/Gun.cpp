#include "Gun.h"

#include <DirectXMath.h>
#include "Engine/Model.h"
#include "Engine/Input.h"

#include "../include/imgui.h"

using namespace DirectX;
using namespace std;
using namespace Config;

namespace
{
    // 銃の初期値
    const XMFLOAT3 Init_Pos[PLAYER_NUM] = { { -3.5f, -7.0f, -6.0f }, { 3.8f, -7.0f, -6.0f } };
    const XMFLOAT3 Init_Rot[PLAYER_NUM] = { { 0.0f, -103.0f, -12.0f }, { 0.0f, -77.0f, -12.0f } };
    const float    Gun_scale = 0.5f; // 拡大率

    // 回転の範囲
    const float Rot_Y_Left_Limit  = -124.0f; // 左へ向ける限界
    const float Rot_Y_Right_Limit =  -60.0f; // 右へ向ける限界
    const float Rot_Z_UP_Limit    =    8.0f; // 上へ向ける限界
    const float Rot_Z_Down_Limit  =  -24.0f; // 下へ向ける限界

    // 回転速度
    const float Rot_Z_Speed = 0.5f; // 上下
    const float Rot_Y_Speed = 0.7f; // 左右
}

// コンストラクタ
Gun::Gun(GameObject* parent)
    :GameObject(parent, "Reticle"),
    position_{ {0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} },
    rotate_  { {0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} },
    scale_   { {0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} },
    hModel_(-1)
{
}

// デストラクタ
Gun::~Gun()
{
}

// 初期化
void Gun::Initialize()
{
    //モデルデータのロード
    hModel_ = Model::Load("/Model/Gun00.fbx");
    assert(hModel_ >= 0);
    
    // 一応あると楽だからここに
    for (int i = 0; i < PLAYER_NUM; i++)
    {
        position_[i] = Init_Pos[i];
        rotate_[i]   = Init_Rot[i];
        scale_[i]    = { Gun_scale, Gun_scale, Gun_scale };
    }

    textGun_.Initialize("Fonts/Rye-Regular.ttf", 28.0f);
}

// 更新
void Gun::Update()
{
    Move();

    // 銃の回転の上限指定
    {
        for (int i = 0; i < PLAYER_NUM; i++) {
            if (rotate_[i].y <= Rot_Y_Left_Limit)  rotate_[i].y = Rot_Y_Left_Limit;  // 左
            if (rotate_[i].y >= Rot_Y_Right_Limit) rotate_[i].y = Rot_Y_Right_Limit; // 右
            if (rotate_[i].z >= Rot_Z_UP_Limit)    rotate_[i].z = Rot_Z_UP_Limit;    // 上
            if (rotate_[i].z <= Rot_Z_Down_Limit)  rotate_[i].z = Rot_Z_Down_Limit;  // 下
        }
    }
}

// 描画
void Gun::Draw()
{
    for (int i = 0; i < PLAYER_NUM; i++)
    {
        transform_.position_ = position_[i]; // 位置
        transform_.rotate_   = rotate_[i];   // 回転
        transform_.scale_    = scale_[i];    // 拡大率

        Model::SetTransform(hModel_, transform_);
        Model::Draw(hModel_);
    }

    // ImGui (デバック用)
    {
        //// ======= 銃の位置座標 =======
        //ImGui::Begin("Gun_position");
        //ImGui::InputFloat("pos.X", &position_[0].x);
        //ImGui::InputFloat("pos.Y", &position_[0].y);
        //ImGui::InputFloat("pos.Z", &position_[0].z);
        //ImGui::End();
        //// ======= 銃の回転 =======
        //ImGui::Begin("Gun_rotate");
        //ImGui::InputFloat("rot.X", &rotate_[0].x);
        //ImGui::InputFloat("rot.Y", &rotate_[0].y);
        //ImGui::InputFloat("rot.Z", &rotate_[0].z);
        //ImGui::End();
        //// ======= 銃の拡大率 =======
        //ImGui::Begin("Gun_scale");
        //ImGui::InputFloat("sca.X", &scale_[0].x);
        //ImGui::InputFloat("sca.Y", &scale_[0].y);
        //ImGui::InputFloat("sca.Z", &scale_[0].z);
        //ImGui::End();
    }
}

void Gun::Release()
{
}

void Gun::Move()
{
    // コントローラー対応
    for (int i = 0; i < PLAYER_NUM; i++)
    {
        //移動ベクトル
        XMFLOAT3 moveVector = Input::GetPadStickL(i);

        // XMFLOAT3 から float への変換が必要
        rotate_[i].z += moveVector.y * Rot_Z_Speed;
        rotate_[i].y += moveVector.x * Rot_Y_Speed;
    }

    // 1P
    if (Input::IsKey(DIK_W)) rotate_[PLAYER_1].z += Rot_Z_Speed; //上
    if (Input::IsKey(DIK_S)) rotate_[PLAYER_1].z -= Rot_Z_Speed; //下
    if (Input::IsKey(DIK_D)) rotate_[PLAYER_1].y += Rot_Y_Speed; //右旋回
    if (Input::IsKey(DIK_A)) rotate_[PLAYER_1].y -= Rot_Y_Speed; //左旋回

    // 2P ※デバッグ用
    if (Input::IsKey(DIK_I)) rotate_[PLAYER_2].z += Rot_Z_Speed; //上
    if (Input::IsKey(DIK_K)) rotate_[PLAYER_2].z -= Rot_Z_Speed; //下
    if (Input::IsKey(DIK_L)) rotate_[PLAYER_2].y += Rot_Y_Speed; //右旋回
    if (Input::IsKey(DIK_J)) rotate_[PLAYER_2].y -= Rot_Y_Speed; //左旋回
}
