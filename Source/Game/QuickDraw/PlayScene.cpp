#include "PlayScene.h"
#include "Engine/Audio.h"
//#include "Player_L.h"
//#include "Player_R.h"
#include "Game01Player.h"
#include "Ground.h"
//#include "Enemy.h"
#include "Infomation.h"
#include "Engine\\FontText.hpp"
#include "Engine\\GUIbutton.h"
#include <Imgui.h>

//コンストラクタ
PlayScene::PlayScene(GameObject * parent)
	: GameObject(parent, "PlayScene")
{
}

//初期化
void PlayScene::Initialize()
{
    bgmHandle_ = Audio::Load("Audio/Game01Bgm00.wav", true);
    Audio::Play(bgmHandle_);
    //左プレイヤー
    leftPlayer_ = new Game01Player(this,"Model/Cowboy01.fbx",-4.0f,90.0f);

    PushBackChild(leftPlayer_);
    leftPlayer_->Initialize();

    //右プレイヤー
    rightPlayer_ = new Game01Player(this,"Model/Cowboy02.fbx",4.0f,270.0f);

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
        "Fonts/aoyagireisyosimo_ttf_2_01.ttf", 62.0f);

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
void PlayScene::Update()
{
}

//描画
void PlayScene::Draw()
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
void PlayScene::Release()
{
    if (bgmHandle_ >= 0)
        Audio::Stop(bgmHandle_);
    playSceneButton_.Reset();
    text_.Release();
    japaneseText_.Release();
}