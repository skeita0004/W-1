#include "PlayScene.h"
#include "Reticle.hpp"
#include "Clay.h"
#include "UI.h"
//#include "Tank.h"
//#include "Ground.h"
//#include "Enemy.h"
//#include "Infomation.h"
#include "Engine\\FontText.hpp"
#include "Gun.h"
#include "Engine/Image.h"

// コンストラクタ
PlayScene::PlayScene(GameObject * parent)
    : GameObject(parent, "PlayScene"), hBackGround_(-1)//, ENEMY_NUM(5)
{
}

// 初期化
void PlayScene::Initialize()
{
    // レティクルの画像のロード
    hBackGround_ = Image::Load("Image/BG.jpg");
    assert(hBackGround_ >= 0);
    Image::SetRect(hBackGround_, 0, 0, 1280, 720);

    //Instantiate<Tank>(this);
    //Instantiate<Ground>(this);
    Instantiate<Clay>(this);
    Instantiate<Reticle>(this);

    // 銃表示（銃はレティクルの子クラス）
    Instantiate<Gun>(this);

    Instantiate<UI>(this);

    //for (int i = 0; i < ENEMY_NUM; i++) {
    //    Instantiate<Enemy>(this);
    //}

    // Instantiate<Infomation>(this);

    textPlay_.Initialize("Fonts/Rye-Regular.ttf", 28.0f);
}

// 更新
void PlayScene::Update()
{
}

// 描画
void PlayScene::Draw()
{
    Image::SetTransform(hBackGround_, transform_);
    Image::Draw(hBackGround_);

    // text_.Draw(32, 32, "Yes!TakasuClinic");
    // textPlay_.Draw(32, 32, "Clay shooting");

    // 弾の残数
    //text_.Draw(80, 80, "x %.f",UI::Bullet);
}

// 開放
void PlayScene::Release()
{
}