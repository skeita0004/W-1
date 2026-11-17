#include "TitleMessage.h"

#include "Engine/Text.h"

#include "Engine/Direct3D.h"

namespace
{
    int counter{};
    float blinkIntervalDiv{1};
}

TitleMessage::TitleMessage(GameObject* _pParent) :
    GameObject(_pParent, "TitleMessage"),
    pText_(nullptr),
    changeScene_(false)
{
}

void TitleMessage::Initialize()
{
    pText_ = new Text();
    pText_->Initialize();

    transform_.position_.y = -1.f;
}

void TitleMessage::Update()
{

    counter++;

    if (changeScene_)
    {
        blinkIntervalDiv = 0.125f;
    }

    if (counter % (int)(BLINK_OUT_COUNT * blinkIntervalDiv) == 0)
    {
        Invisible();
    }
    else if (not(IsVisibled()) and counter % (int)(LIGHT_UP_COUNT * blinkIntervalDiv) == 0)
    {
        Visible();
    }
}

void TitleMessage::Draw()
{
    std::string titleMessage{"Push A button to Play."};

    int offset = 16 * titleMessage.length();

    pText_->Draw((Direct3D::screenWidth_ / 2) - offset / 2,
                  pText_->GetScreenPos(transform_).y,
                  titleMessage.c_str());
}

void TitleMessage::Release()
{
    counter = 0;
    blinkIntervalDiv = 1;
}
