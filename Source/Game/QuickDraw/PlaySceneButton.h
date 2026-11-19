
#pragma once

#include "Engine/GuiButton.h"
#include "Engine/FontText.hpp"
#include "Engine/GameTimer.h"
#include "ControllerTest.h"
#include <chrono>
#include <memory>
#include <string>

class PlaySceneButton
{
public:
    PlaySceneButton() = default;
    ~PlaySceneButton() = default;

    PlaySceneButton(const PlaySceneButton&) = delete;

    PlaySceneButton& operator=(const PlaySceneButton&) = delete;

    void Draw();

    void SetScreenSize(float screenWidth, float screenHeight);

    void Reset();

    bool IsGameFinished() const
    {
        return gameFinished_;
    }

    int GetWinnerSide() const
    {
        return winnerSide_;
    }

    //
    bool Initialize(float screenWidth, float screenHeight,
        FontText* font, FontText* japaneseFont, GameTimer* timer);

private:
    void OnStartClicked();
    void OnStopClicked(int side);
    void UpdateButtonPositions();
    void ResetRound(bool restartTimer);

    float screenWidth_ = 800.0f;
    float screenHeight_ = 600.0f;
    FontText* font_ = nullptr;
    FontText* japaneseFont_ = nullptr;
    GameTimer* timer_ = nullptr;
    int buttonSeHandle_ = -1;
    int winnerSeHandle_ = -1;
    int winnerFollowupSeHandle_ = -1;
    int falseStartSeHandle_ = -1;
    int countdownSeHandle_ = -1;
    bool winnerFollowupPending_ = false;
    std::chrono::steady_clock::time_point winnerFollowupTime_{};

    //フライング表示用
    bool falseStartPending_ = false;
    std::chrono::steady_clock::time_point falseStartMessageEndTime_{};

    std::unique_ptr<GuiButton> startButton_;
    std::unique_ptr<GuiButton> leftStopButton_;
    std::unique_ptr<GuiButton> rightStopButton_;

    bool leftPressed_ = false;
    bool rightPressed_ = false;
    bool hasResult_ = false;
    bool gameFinished_ = false;

    double leftTime_ = 0.0;
    double rightTime_ = 0.0;
    int leftFalseStarts_ = 0;
    int rightFalseStarts_ = 0;
    int winnerSide_ = 0; //-1: 左、1: 右、0: 引き分け
    std::string resultText_;

    std::chrono::steady_clock::time_point countdownStartTime_{};

    ControllerTest controllerTest_; //コントローラーのテスト用
};
