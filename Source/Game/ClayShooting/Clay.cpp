// Engine
#include "Input.hpp"
#include "Model.hpp"
#include "Audio.hpp"
#include "SceneManager.hpp"
#include "CameraSet.hpp"

// Game
#include "Clay.hpp"

// その他
#include <DirectXMath.h>
#include "imgui.h"

using namespace DirectX;
using namespace Config;

// staticメンバの実体 ※これ書かないとスコア加算されない
int Clay::TotalScore_[PLAYER_NUM] = {};

namespace
{
    // ====== これがないとリンカーエラー吐く（AI）==========
    // ※ 画面のアスペクト比に合わせて変更すること
    const float CAM_ASPECT = 16.0f / 9.0f;

    // 通常カメラのビュー・プロジェクション行列を作る
    void GetNormalCameraMatrix(XMMATRIX* pView, XMMATRIX* pProj)
    {
        IVirtualCamera* camera = CameraSet::GetVirtualCamera(CameraSet::hNormalCamera);
        Transform& tr = camera->GetTransform();

        // ビュー行列 = カメラのワールド行列の逆行列
        XMMATRIX world =
            XMMatrixRotationRollPitchYaw(XMConvertToRadians(tr.rotate.x),
                                         XMConvertToRadians(tr.rotate.y),
                                         XMConvertToRadians(tr.rotate.z))
            * XMMatrixTranslation(tr.position.x, tr.position.y, tr.position.z);
        *pView = XMMatrixInverse(nullptr, world);

        // CameraSet::Initialize と同じ値（視野角120度、near 0.1、far 1000）
        *pProj = XMMatrixPerspectiveFovLH(XMConvertToRadians(120.0f),
                                          CAM_ASPECT, 0.1f, 1000.0f);
    }
    // =====================================================

    // クレーの初期値
    const float CLAY_SCALE = 0.2f;   // 拡大率（XYZ共通）
    const int   CLAY_SCORE = 50;     // 1枚あたりのスコア

    // クレー0～2の初期位置
    /*
    const XMFLOAT3 Init_Pos[Clay_Max] = {
        {  0.0f, 3.0f, 1.0f },
        {  0.8f, 1.0f, 0.0f },
        { -0.4f, 0.0f, 0.2f }
    };
    */
    // クレーの初期値（仮）
    const XMFLOAT3 Init_Pos[6] 
        = {{0.0f,3.0f,1.0f},{0.8f,1.0f,0.0f},{-0.4f,0.0f,0.2f},
           {0.2f,2.0f,0.0f},{-0.2f,0.0f,0.2f},
           {0.4f,1.0f,-1.0f}};
    const XMFLOAT3 Init_Rot[6]
        = {{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f},
           {0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f},
           {0.0f,0.0f,0.0f}};
    const XMFLOAT3 Init_Sca[6]
        = {{0.2f,0.2f,0.2f},{0.2f,0.2f,0.2f},{0.2f,0.2f,0.2f},
           {0.2f,0.2f,0.2f},{0.2f,0.2f,0.2f},
           {0.2f,0.2f,0.2f}};

    // 移動範囲
    const float Field_Left  = -12.0f;  // これより左へ行くと右端へ戻る
    const float Field_Right = 12.0f;   // これより右へ行くと左端へ戻る
    const float Field_Up    = 2.5f;    // 上限（ここで下降に切り替え）
    const float Field_Down  = -2.5f;   // 下限（ここで上昇に切り替え）
}

//コンストラクタ
Clay::Clay(GameObject* parent)
    :GameObject(parent, "Clay"),
    position_ {{ 0.0f,0.0f,0.0f }}, // 位置
    rotate_   {{ 0.0f,0.0f,0.0f }}, // 回転
    scale_    {{ 0.0f,0.0f,0.0f }}, // 拡大率
    HitRange  {(0.5f)}, // 当たり判定の大きさ
    hModel_   {(-1)},   // モデル番号
    hSound_   {(-1)},   // サウンド読み込み
    OnDown_   {true},   // 下がるか上がるかのフラグ　    ※ true：下がる / false：上がる
    IsDead_   {false},  // クレーの死亡しているかフラグ  ※ true：死亡 / false：生存
    Score_    {(15,15,15,25,25,-50, 0)}  // 各種スコア
{
}

//デストラクタ
Clay::~Clay()
{
}

//初期化
void Clay::Initialize()
{
    // ここで合計スコアをリセット
    ScoreReset();

    // モデルデータのロード
    {
        hModel_[Clay_A]   
            = Model::Load("Model/ClayShooting/Dish00.fbx");
        hModel_[Clay_B]   
            = Model::Load("Model/ClayShooting/Dish00.fbx");
        hModel_[Clay_C]   
            = Model::Load("Model/ClayShooting/Dish00.fbx");
        hModel_[Bottle_A] 
            = Model::Load("Model/ClayShooting/bin_mock.fbx");
        hModel_[Bottle_B] 
            = Model::Load("Model/ClayShooting/bin_mock.fbx");
        hModel_[Bomb]     
            = Model::Load("Model/ClayShooting/Bomb00.fbx");

        assert(hModel_[Clay_A] >= 0);
        assert(hModel_[Clay_B] >= 0);
        assert(hModel_[Clay_C] >= 0);
        assert(hModel_[Bottle_A] >= 0);
        assert(hModel_[Bottle_B] >= 0);
        assert(hModel_[Bomb] >= 0);

        /*
        for (int i = 0; i < Clay_Max; i++)
        {
            hModel_[i] = Model::Load("/Model/Dish00.fbx");
            assert(hModel_[i] >= 0);
        }
        */
    }

    // サウンドデータのロード
    {
        hSound_[Clay_A]
            = Audio::Load("Audio/ClayShooting/ガラスが割れる3.wav"); // クレーが壊れる音
        hSound_[Clay_B]
            = Audio::Load("Audio/ClayShooting/ガラスが割れる3.wav"); // クレーが壊れる音
        hSound_[Clay_C]
            = Audio::Load("Audio/ClayShooting/ガラスが割れる3.wav"); // クレーが壊れる音
        hSound_[Bottle_A]
            = Audio::Load("Audio/ClayShooting/空き瓶が砕ける.wav");  // 空き瓶が割れる音
        hSound_[Bottle_B]
            = Audio::Load("Audio/ClayShooting/空き瓶が砕ける.wav");  // 空き瓶が割れる音
        hSound_[Bomb]
            = Audio::Load("Audio/ClayShooting/爆発1.wav");           // 爆弾の爆発音

        assert(hSound_[Clay_A] >= 0);
        assert(hSound_[Clay_B] >= 0);
        assert(hSound_[Clay_C] >= 0);
        assert(hSound_[Bottle_A] >= 0);
        assert(hSound_[Bottle_B] >= 0);
        assert(hSound_[Bomb] >= 0);

        /*
        for (int i = 0; i < Clay_Max; i++)
        {
            hSound_[i] = Audio::Load("Audio/ガラスが割れる3.wav"); // クレーが壊れる音
            assert(hSound_[i] >= 0);
        }
        */
    }

    // 
    {

    }
    
    // 一応あると楽だからここに
    // === 位置のメモ ===
    // (float)(rand() % 10 - 5)
    // X - 1.2～1.2
    // Y 2.0～3.0
    // Z - 6.5～ - 8.0

    /*
    position_[0] = { 0.0f, 3.0f, 1.0f }; // 位置
    rotate_[0] = { 0.0f, 0.0f, 0.0f }; // 回転
    scale_[0] = { 0.2f, 0.2f, 0.2f }; // 拡大率
    Score_[0] = 50; // スコア

    position_[1] = { 0.8f, 1.0f, 0.0f }; // 位置
    rotate_[1] = { 0.0f, 0.0f, 0.0f }; // 回転
    scale_[1] = { 0.2f, 0.2f, 0.2f }; // 拡大率
    Score_[1] = 50; // スコア

    position_[2] = { -0.4f, 0.0f, 0.2f }; // 位置
    rotate_[2] = { 0.0f, 0.0f, 0.0f }; // 回転
    scale_[2] = { 0.2f, 0.2f, 0.2f }; // 拡大率
    Score_[2] = 50; // スコア
    */
    
    // デバック用
    {
        /*
        for (int i = 3; i < Clay_Max; i++)
        {
            position_[i] = { 1.0f + ((float)(i - 3) * 1.0f), -2.5f + ((float)(i - 3) * 1.0f), 0.0f }; // 位置
            rotate_[i] = { 0.0f, 0.0f, 0.0f }; // 回転
            scale_[i] = { 0.2f, 0.2f, 0.2f }; // 拡大率
            Score_[i] = 50; // スコア
        }
        */

        /*
        position_[3] = { 0.0f, 1.0f, 0.0f }; // 位置
        rotate_[3] = { 0.0f, 0.0f, 0.0f }; // 回転
        scale_[3] = { 0.2f, 0.2f, 0.2f }; // 拡大率
        Score_[3] = 50; // スコア

        position_[4] = { 0.0f, 1.2f, 0.0f }; // 位置
        rotate_[4] = { 0.0f, 0.0f, 0.0f }; // 回転
        scale_[4] = { 0.2f, 0.2f, 0.2f }; // 拡大率
        Score_[4] = 50; // スコア

        position_[5] = { 0.0f, 0.8f, 0.0f }; // 位置
        rotate_[5] = { 0.0f, 0.0f, 0.0f }; // 回転
        scale_[5] = { 0.2f, 0.2f, 0.2f }; // 拡大率
        Score_[5] = 50; // スコア

        position_[6] = { 0.0f, 1.4f, 0.0f }; // 位置
        rotate_[6] = { 0.0f, 0.0f, 0.0f }; // 回転
        scale_[6] = { 0.2f, 0.2f, 0.2f }; // 拡大率
        Score_[6] = 50; // スコア

        position_[7] = { 0.0f, 0.6f, 0.0f }; // 位置
        rotate_[7] = { 0.0f, 0.0f, 0.0f }; // 回転
        scale_[7] = { 0.2f, 0.2f, 0.2f }; // 拡大率
        Score_[7] = 50; // スコア

        position_[8] = { 0.0f, 1.6f, 0.0f }; // 位置
        rotate_[8] = { 0.0f, 0.0f, 0.0f }; // 回転
        scale_[8] = { 0.2f, 0.2f, 0.2f }; // 拡大率
        Score_[8] = 50; // スコア

        position_[9] = { 0.0f, 0.4f, 0.0f }; // 位置
        rotate_[9] = { 0.0f, 0.0f, 0.0f }; // 回転
        scale_[9] = { 0.2f, 0.2f, 0.2f }; // 拡大率
        Score_[9] = 50; // スコア
        */
    }

    //当たり判定追加
    //SphereCollider* collision = new SphereCollider(XMFLOAT3(0.0f, 0.1f, 0.0f), 0.8f);
    //AddCollider(collision);

    textClay_.Initialize("../Fonts/Rye-Regular.ttf", 28.0f);
}

//更新
void Clay::Update()
{
    Move();

    // 銃の回転の上限指定
    {
        /*
        // クレー１
        {
            if (position_[0].x < Field_Left) position_[0].x = Field_Right; // 左から右
            if (position_[0].x > Field_Right) position_[0].x = Field_Left; // 右から左
        }

        // クレー２
        {
            if (position_[1].x < Field_Left) position_[1].x = Field_Right; // 左から右
            if (position_[1].x > Field_Right) position_[1].x = Field_Left; // 右から左
            if (position_[1].y > Field_Up) {
                OnDown_[1] = true; // 降下中
                position_[1].y = Field_Up; // 上から下
            }
            if (position_[1].y < Field_Down) {
                OnDown_[1] = false; // 上昇
                position_[1].y = Field_Down; // 下から上
            }
        }

        // クレー３
        {
            if (position_[2].x < Field_Left) position_[2].x = Field_Right; // 左から右
            if (position_[2].x > Field_Right) position_[2].x = Field_Left; // 右から左
            if (position_[2].y > Field_Up) {
                OnDown_[2] = true; // 降下中
                position_[2].y = Field_Up; // 上から下
            }
            if (position_[2].y < Field_Down) {
                OnDown_[2] = false; // 上昇
                position_[2].y = Field_Down; // 下から上
            }
        }
        */

        // デバック用クレー
        {
            /*
            for (int i = 3; i < Clay_Max; i++)
            {
                if (position_[i].x < Field_Left) position_[i].x = Field_Right; // 左から右
                if (position_[i].x > Field_Right) position_[i].x = Field_Left; // 右から左
            }
            */
            /*
            if (position_[3].x < Field_Left) position_[3].x = Field_Right; // 左から右
            if (position_[3].x > Field_Right) position_[3].x = Field_Left; // 右から左

            if (position_[4].x < Field_Left) position_[4].x = Field_Right; // 左から右
            if (position_[4].x > Field_Right) position_[4].x = Field_Left; // 右から左

            if (position_[5].x < Field_Left) position_[5].x = Field_Right; // 左から右
            if (position_[5].x > Field_Right) position_[5].x = Field_Left; // 右から左

            if (position_[6].x < Field_Left) position_[6].x = Field_Right; // 左から右
            if (position_[6].x > Field_Right) position_[6].x = Field_Left; // 右から左

            if (position_[7].x < Field_Left) position_[7].x = Field_Right; // 左から右
            if (position_[7].x > Field_Right) position_[7].x = Field_Left; // 右から左

            if (position_[8].x < Field_Left) position_[8].x = Field_Right; // 左から右
            if (position_[8].x > Field_Right) position_[8].x = Field_Left; // 右から左

            if (position_[9].x < Field_Left) position_[9].x = Field_Right; // 左から右
            if (position_[9].x > Field_Right) position_[9].x = Field_Left; // 右から左
            */
        }

    }
}

void Clay::Move()
{

    /*
    position_[Clay_A].x += 0.08f;
    rotate_[Clay_A].x += 2.5f;
    rotate_[Clay_A].y += 5.0f;

    position_[Clay_B].x += 0.08f;
    rotate_[Clay_B].x += 2.5f;
    rotate_[Clay_B].y += 5.0f;

    position_[Clay_C].x += 0.08f;
    rotate_[Clay_C].x += 2.5f;
    rotate_[Clay_C].y += 5.0f;

    position_[Bottle_A].x -= 0.10f;
    rotate_[Bottle_A].x -= 1.0f;
    if (OnDown_[Bottle_A]) position_[Bottle_A].y -= 0.02f;
    else position_[Bottle_A].y += 0.02f;

    position_[Bottle_B].x -= 0.10f;
    rotate_[Bottle_B].x -= 1.0f;
    if (OnDown_[Bottle_B]) position_[Bottle_B].y -= 0.02f;
    else position_[Bottle_B].y += 0.02f;

    position_[Bomb].x += 0.15f;
    if (OnDown_[Bomb]) position_[Bomb].y -= 0.08f;
    else position_[Bomb].y += 0.08f;
    */

    /*
    for (int i = 3; i < Clay_Max; i++)
    {
        position_[i].x += 0.08f;
        rotate_[i].x += 2.5f;
        rotate_[i].y += 5.0f;
    }
    */

}

//描画
void Clay::Draw()
{
    // クレー表示
    for (int i = 0; i < ClayType_Max; i++)
    {
        // 各クレーが破壊されていなければ描画
        if (IsDead_[i]) continue;
        transform_.position = position_[i]; // 位置
        transform_.rotate = rotate_[i];   // 回転
        transform_.scale = scale_[i];    // 拡大率

        Model::SetTransform(hModel_[i], transform_);
        Renderer::Draw(hModel_[i]);
    }

    // ImGui (デバック用)
    {
        //// ======= クレーの位置座標 =======
        //ImGui::Begin("Clay_position");
        //ImGui::InputFloat("pos.X1", &position_[0].x);
        //ImGui::InputFloat("pos.Y1", &position_[0].y);
        //ImGui::InputFloat("pos.Z1", &position_[0].z);
        //ImGui::InputFloat("pos.X2", &position_[1].x);
        //ImGui::InputFloat("pos.Y2", &position_[1].y);
        //ImGui::InputFloat("pos.Z2", &position_[1].z);
        //ImGui::InputFloat("pos.X3", &position_[2].x);
        //ImGui::InputFloat("pos.Y3", &position_[2].y);
        //ImGui::InputFloat("pos.Z3", &position_[2].z);
        //ImGui::End();
    }
}

void Clay::Release()
{
}

/*
void Clay::OnCollision(GameObject* pTarget)
{
    //弾に当たった
    if (pTarget->GetObjectName() == "Bullet")
    {
        //自分を消す
        KillMe();
    }
}
*/

bool Clay::AllBroken() const
{
    for (int i = 0; i < Clay_Max; i++)
    {
        if (!IsDead_[i]) return false;
    }
    return true;
}

int Clay::FindOverlap(const XMFLOAT2& reticleNdc) const
{
    //const auto& camera{CameraSet::GetVirtualCamera(CameraSet::hNormalCamera)};

    //XMMATRIX view = CameraSet::GetViewMatrix();
    //XMMATRIX proj = CameraSet::GetProjectionMatrix();
    
    //ビュー行列を取得(前の遠藤エンジンから持ってきた)
    //XMMATRIX GetViewMatrix();

    //プロジェクション行列を取得(前の遠藤エンジンから持ってきた)
    //XMMATRIX GetProjectionMatrix();

    XMMATRIX view, proj;
    GetNormalCameraMatrix(&view, &proj);

    //const float hitSize = 0.05f;
    int   hit = -1;
    float hitDepth = FLT_MAX;

    for (int i = 0; i < Clay_Max; i++)
    {
        if (IsDead_[i]) continue;

        // ワールド座標をビュー座標に変換
        XMVECTOR world = XMVector3TransformCoord(XMLoadFloat3(&position_[i]), view);
        float depth = XMVectorGetZ(world);
        if (depth <= 0.01f) continue; // カメラの後ろは対象外

        // ビュー座標から画面座標に変換
        XMVECTOR center = XMVector3TransformCoord(world, proj);
        XMVECTOR edgeX = XMVector3TransformCoord(XMVectorAdd(world, XMVectorSet(HitRange, 0.0f, 0.0f, 0.0f)), proj);
        XMVECTOR edgeY = XMVector3TransformCoord(XMVectorAdd(world, XMVectorSet(0.0f, HitRange, 0.0f, 0.0f)), proj);

        float cx = XMVectorGetX(center);
        float cy = XMVectorGetY(center);
        float rx = fabsf(XMVectorGetX(edgeX) - cx);
        float ry = fabsf(XMVectorGetY(edgeY) - cy);

        // レティクル中心の確認
        float dx = (reticleNdc.x - cx) / rx;
        float dy = (reticleNdc.y - cy) / ry;
        if ((dx * dx + dy * dy) <= 1.0f) {
            // 複数重なっていたら、カメラに近いものを優先的に破壊
            if (depth < hitDepth) {
                hitDepth = depth;
                hit = i;
            }
        }
    }
    return hit;
}

bool Clay::TryBreak(int player, const XMFLOAT2& reticleNdc)
{
    if (player < 0 || player >= PLAYER_NUM) return false;

    int hit = FindOverlap(reticleNdc);
    if (hit < 0) return false;

    // 命中したクレーを壊す
    IsDead_[hit] = true;

    // 合計スコアに加算
    TotalScore_[player] += Score_[hit];

    // 全部壊れたら自分を消す
    if (AllBroken()) {
        Audio::Play(hSound_[hit]);
        KillMe();
    }

    return true;
}
