// Engine
#include "Input.hpp"
#include "Image.hpp"
#include "Audio.hpp"
#include "SceneManager.hpp"
#include "CameraSet.hpp"
#include "FontText.hpp"

// Game
#include "PlayScene.hpp"
#include "Reticle.hpp"
#include "UI.hpp"
#include "Gun.hpp"
#include "Clay.hpp"

// その他
#include <DirectXMath.h>
#include "imgui.h"

using namespace DirectX;
using namespace Config;
using namespace std;

namespace
{
    const int str_num = 64;

    // UIの初期値(一応あると楽だからここに)
    const XMFLOAT3 Init_Pos[2] // 位置
        = {{0.0f,0.0f,0.0f}};
    const XMFLOAT3 Init_Rot[2] // 回転
        = {{0.0f,0.0f,0.0f}};
    const XMFLOAT3 Init_Sca[2] // 拡大率
        = {{1.0f,1.0f,1.0f}};
}

// コンストラクタ
PlayScene::PlayScene(GameObject * parent)
    : GameObject(parent, "PlayScene"),
    position_{{ 0.0f,0.0f,0.0f }}, // 位置
    rotate_  {{ 0.0f,0.0f,0.0f }}, // 回転
    scale_   {{ 0.0f,0.0f,0.0f }}, // 拡大率
    hModel_  {(-1)}, // モデル読み込み
    hSound_  {(-1)}, // サウンド読み込み
    hBackGround_(-1) // 背景画像
    //, ENEMY_NUM(5)
{
}

// 初期化
void PlayScene::Initialize()
{
    // レティクルの画像のロード
    {
        hBackGround_
            = Image::Load("../Image/ClayShooting/BG.jpg");

        assert(hBackGround_ >= 0);

        Image::SetRect(hBackGround_, 0, 0, 1280, 720);
    }

    // 背景用モデルのロード
    {
        hModel_[Ground]
            = Model::Load("../Model/ClayShooting/oden.fbx");

        assert(hModel_[Ground] >= 0);
    }

    // サウンドデータのロード
    {
        hSound_[Wind]
            = Audio::Load("../Audio/ClayShooting/風が吹く1.wav"); // 風の音

        assert(hSound_[Wind] >= 0);
    }

    Instantiate<Clay>(this);
    Instantiate<Reticle>(this);
    Instantiate<Gun>(this);
    Instantiate<UI>(this);


    textPlay_.Initialize("../Fonts/Rye-Regular.ttf", 28.0f);
}

// 更新
void PlayScene::Update()
{
    // SEを常時流す
    Audio::Play(hSound_[Wind]);
}

// 描画
void PlayScene::Draw()
{
    // 背景画像の配置
    Image::SetTransform(hBackGround_, transform_);
    Image::Draw(hBackGround_);

    // 地面設置
    transform_.position = {Init_Pos[Ground]};  // 位置
    transform_.scale    = {Init_Sca[Ground]};  // 拡大率
    Model::SetTransform(hModel_[Ground], transform_);
    Renderer::Draw(hModel_[Ground]);


    // text_.Draw(32, 32, "Yes!TakasuClinic");
    // textPlay_.Draw(32, 32, "Clay shooting");

    // 弾の残数
    //text_.Draw(80, 80, "x %.f",UI::Bullet);
}

// 開放
void PlayScene::Release()
{
}