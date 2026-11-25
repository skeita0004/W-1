#include "01PlayScene.hpp"
#include "Audio.hpp"
//#include "Player_L.hpp"
//#include "Player_R.hpp"
#include "Game01Player.hpp"
#include "01Ground.hpp"
//#include "Enemy.hpp"
#include "01Infomation.hpp"
#include "FontText.hpp"
#include "GUIbutton.hpp"
#include <Imgui.h>
#include <cassert>
#include <CameraSet.hpp>
#include "QuickDrawSky.hpp"

//コンストラクタ
QuickDrawScene::QuickDrawScene(GameObject * parent)
	: GameObject(parent, "PlayScene")
{
}

//初期化
void QuickDrawScene::Initialize()
{
    //カメラ設定
    auto camera = CameraSet::GetCurrent();
    camera->GetTransform().position = XMFLOAT3(0.0f, 0.8f, -4.0f);
    camera->GetTargetTransform().position = XMFLOAT3(0.0f, 0.0f, 0.0f);

    //背景
    sky_ = new QuickDrawSky(this);
    PushBackChild(sky_);
    sky_->Initialize();

    bgmHandle_ = Audio::Load("Audio/QuickDraw/Game01Bgm00.wav", true);
    Audio::Play(bgmHandle_);
    //左プレイヤー
    leftPlayer_ = new Game01Player(this,"Models/QuickDraw/Cowboy01.fbx",-4.0f,90.0f);

    PushBackChild(leftPlayer_);
    leftPlayer_->Initialize();

    //右プレイヤー
    rightPlayer_ = new Game01Player(this,"Models/QuickDraw/Cowboy02.fbx",4.0f,270.0f);

    PushBackChild(rightPlayer_);
    rightPlayer_->Initialize();

    //地面
    Instantiate<Ground>(this);

    //UI情報
    Instantiate<Infomation>(this);

    //英数字用フォント
    text_.Initialize("Fonts/Rye-Regular.ttf", 42.0f);

    //日本語用フォント
    japaneseText_.Initialize(
        "Fonts/aoyagireisyosimo_ttf_2_01.ttf", 42.0f);

    const ImVec2 size = ImGui::GetIO().DisplaySize;
    
    playSceneButton_.Initialize(
        size.x,
        size.y,
        &text_,
        &japaneseText_,
        &timer_
    );

}

//更新
void QuickDrawScene::Update()
{
}

//描画
void QuickDrawScene::Draw()
{
    const ImVec2 size = ImGui::GetIO().DisplaySize;
    playSceneButton_.SetScreenSize(size.x, size.y);
    playSceneButton_.Draw();

    //試合終了時の処理
    if (playSceneButton_.IsGameFinished())
    {
        const int winner = playSceneButton_.GetWinnerSide();

        //左が勝利 → 右が倒れる
        if (winner == -1)
        {
            leftPlayer_->SetFallen(false);
            rightPlayer_->SetFallen(true);
        }
        //右が勝利 → 左が倒れる
        else if (winner == 1)
        {
            leftPlayer_->SetFallen(true);
            rightPlayer_->SetFallen(false);
        }
        //引き分け
        else
        {
            leftPlayer_->SetFallen(false);
            rightPlayer_->SetFallen(false);
        }
    }
    else
    {
        //試合中は両者起立
        leftPlayer_->SetFallen(false);
        rightPlayer_->SetFallen(false);
    }
}

//開放
void QuickDrawScene::Release()
{
    if (bgmHandle_ >= 0)
        Audio::Stop(bgmHandle_);
    playSceneButton_.Reset();
    text_.Release();
    japaneseText_.Release();
}