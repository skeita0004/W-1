#include "TitleLogo.h"

#include "Engine/Image.h"

TitleLogo::TitleLogo(GameObject* _pParent) :
    GameObject(_pParent, "TitleLogo"),
    hImage_(-1)
{
}

void TitleLogo::Initialize()
{
    hImage_ = Image::Load("Picture/title_logo.png");
    SetScale(10.0f);
    SetPosition(0.0f, 0.5f, 0.0f);
}

void TitleLogo::Update()
{
}

void TitleLogo::Draw()
{
    Image::SetTransform(hImage_, transform_);
    Image::Draw(hImage_);
}

void TitleLogo::Release()
{
}
