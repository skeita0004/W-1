#include "TitleScene.h"

#include "Engine/SceneManager.h"
#include "Engine/Text.h"

#include "Engine/Input.h"
#include "Engine/Image.h"

#include "BackGround.h"
#include "TitleLogo.h"
#include "TitleMessage.h"

namespace
{
    float timer{};
    float waitTimer{1.0f};
}

TitleScene::TitleScene(GameObject* _pParent) :
    pSceneManager_(nullptr),
    isEnterSceneChange_(false),
    pTitleMessage_(nullptr)
{
}

void TitleScene::Initialize()
{
    Instantiate<BackGround>(this);
    Instantiate<TitleLogo>(this);
    pTitleMessage_ = Instantiate<TitleMessage>(this);

    pSceneManager_ = static_cast<SceneManager*>(FindObject("SceneManager"));

}

void TitleScene::Update()
{
    waitTimer -= deltaTime;
    if (waitTimer < 0.f)
    {
        for (size_t i = 0; i < Input::MAX_PAD_NUM; i++)
        {
            if (Input::IsPadButtonDown(XINPUT_GAMEPAD_A, i) or
                Input::IsKeyDown(DIK_SPACE))
            {
                isEnterSceneChange_ = true;
                pTitleMessage_->ChangeScene() = true;
            }
        }

        if (isEnterSceneChange_)
        {
            timer += deltaTime / 1000;
            if (timer > 3.f)
            {
                pSceneManager_->ChangeScene(SCENE_ID_CHICKEN_RACE);
            }   
        }   
    }
}

void TitleScene::Draw()
{
}

void TitleScene::Release()
{
    timer = 0;
    waitTimer = 1.0f;
}
