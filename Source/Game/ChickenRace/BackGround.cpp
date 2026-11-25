#include "BackGround.hpp"

#include "Image.hpp"

BackGround::BackGround(GameObject* _pParent) :
    GameObject(_pParent, "BackGround"),
    hImage_(-1)
{
}

void BackGround::Initialize()
{
    hImage_ = Image::Load("Picture/ChickenRace/back_ground.png");
    transform_.scale = XMFLOAT3(1.75f, 1.75f, 1.f);
}

void BackGround::Update()
{
}

void BackGround::Draw()
{
    Image::SetTransform(hImage_, transform_);
    Image::Draw(hImage_);
}

void BackGround::Release()
{
}
