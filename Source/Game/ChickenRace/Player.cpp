#include "Player.h"

#include <imgui.h>

#include "Engine/InputBinding.h"

#include "Bin.h"
#include "Beermug.h"
#include "Judge.h"

#include "Engine/Text.h"
#include "Engine/Camera.h"
#include "Engine/Direct3D.h"
#include "Engine/Global.h"

namespace
{
    const float OVER_FLOW_TEXT_VISIBLE_TIME{0.3f};
    const float OVER_FLOW_TEXT_INVISIBLE_TIME{0.2f};
}

Player::Player(GameObject* _pParent) :
    GameObject(_pParent, "Player"),
    inputSet_(),
    pBin_(),
    pBeermug_(),
    playerID_(0),
    overFlowTextVisible_(true),
    overFlowTextTimer_(OVER_FLOW_TEXT_VISIBLE_TIME)
{
}

void Player::Initialize()
{
    pText_ = new Text();
    pText_->Initialize();

    playerID_ = playerCount_ + 1;
    objectName_ = "Player" + std::to_string(playerID_);

    pBin_ = Instantiate<Bin>(this);
    pBin_->SetObjectName("Bin" + std::to_string(playerID_));
    pBin_->SetRotateZ(-20.f);
    pBin_->SetScale(0.1f);

    // Transform parent is Table
    pBeermug_ = Instantiate<Beermug>(GetParent());
    pBeermug_->SetObjectName("Beermug" + std::to_string(playerID_));
    pBeermug_->SetPosition(-0.25, 0, 0);
    if (playerID_ == 2)
    {
        pBeermug_->SetPosition(0.25, 0, 0);
        pBeermug_->SetModelIndex(Beermug::BeermugVariation::BEERMUG_CROSS_SECTION_R);
    }
    pBeermug_->SetScale(0.15f);

    playerCount_ += 1;
}

void Player::Update()
{
    bool stickInputPouring{ Input::GetPadStickL(playerID_ - 1).y >= 0.5f };
    if (IsHold(inputSet_.at(PlayerAction::POUR)) or stickInputPouring)
    {
        pBin_->Pouring();
    }

    if (pBeermug_->GetWaterLevel() > Beermug::wlhSpawnLevel and not(pBeermug_->IsOverFlow()))
    {
        bool stickInputConform{ Input::GetPadStickL(playerID_ - 1).y <= -0.5f };
        if ((IsDown(inputSet_.at(PlayerAction::CONFIRMED)) and not(pBin_->IsConfirmed())) or
            stickInputConform and not(pBin_->IsConfirmed()))
        {
            pBin_->Conform();
            Judge::confirmedCount += 1;
        }
    }

    if (pBeermug_->IsOverFlow() and not(pBin_->IsConfirmed()))
    {
        pBin_->Conform();
        Judge::confirmedCount += 1;
    }

    if (pBeermug_->IsOverFlow())
    {
        overFlowTextTimer_ -= deltaTime / 1000.f;
        if (overFlowTextTimer_ <= 0.f)
        {
            overFlowTextVisible_ = not overFlowTextVisible_;
            
            if (overFlowTextVisible_)
            {
                overFlowTextTimer_ = OVER_FLOW_TEXT_INVISIBLE_TIME;
            }
            else
            {
                overFlowTextTimer_ = OVER_FLOW_TEXT_VISIBLE_TIME;
            }
        }
    }
}

void Player::Draw()
{
    XMFLOAT2 textPos{};

    textPos = pText_->GetScreenPos(transform_);
    if (playerID_ == 2)
    {
        textPos.x -= 300.f;
    }
    else
    {
        textPos.x += 50.f;
    }
    textPos.y -= 250.f;
    pText_->Draw((int)textPos.x, (int)textPos.y, objectName_.c_str());

    if (pBeermug_->IsOverFlow() and overFlowTextVisible_)
    {
        textPos.y += 30;
        pText_->Draw( textPos.x, textPos.y, "Oh no" );
        textPos.y += 30;
        pText_->Draw( textPos.x, textPos.y, "It's overflowing!" );
    }

    if (pBeermug_->GetWaterLevel() > 1.0f and 
        not(pBeermug_->IsOverFlow())      and
        pBin_->IsConfirmed())
    {
        textPos.y += 30;
        pText_->Draw( textPos.x, textPos.y, "Oh no" );
        textPos.y += 30;
        pText_->Draw( textPos.x, textPos.y, "It's surface tension!" );
    }
}

void Player::Release()
{
    playerCount_ = 0;
}

float Player::GetWaterLevel()
{
    return pBeermug_->GetWaterLevel();
}

bool Player::IsOverFlow()
{
    return pBeermug_->IsOverFlow();
}
