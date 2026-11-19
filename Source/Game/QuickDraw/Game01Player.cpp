
#include "Game01Player.h"
#include "Engine/Model.h"

#include <cassert>

Game01Player::Game01Player(GameObject* parent,const std::string& modelPath,float posX,float rotateY)
    : GameObject(parent, "Game01Player"),modelPath_(modelPath),defaultRotateY_(rotateY)
{
    //初期位置
    transform_.position_ = XMFLOAT3(posX,0.0f,-1.0f);

    //拡大率
    transform_.scale_ = XMFLOAT3(0.015f,0.015f,0.015f);

    //初期回転
    transform_.rotate_ = XMFLOAT3(0.0f,defaultRotateY_,0.0f);
}

void Game01Player::Initialize()
{
    //モデル読み込み
    hModel_ = Model::Load(modelPath_);

    assert(hModel_ >= 0);
}

void Game01Player::Update()
{
}

void Game01Player::Draw()
{
    if (hModel_ < 0)
        return;

    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

void Game01Player::Release()
{
}

void Game01Player::SetFallen(bool fallen)
{
    if (isFallen_ == fallen)
        return;

    isFallen_ = fallen;

    //X軸に270度倒す
	transform_.rotate_ = XMFLOAT3(fallen ? 270.0f : 0.0f, defaultRotateY_, 0.0f);
}
