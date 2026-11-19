
#include "PlaySceneButton.h"
#include "Engine/Audio.h"
#include "Engine/Input.h"
#include <imgui.h>
#include <algorithm>
#include <cstdio>
#include <random>
#include <cmath>
#include <string>

//初期化
bool PlaySceneButton::Initialize(
    float screenWidth, float screenHeight,
    FontText* font, FontText* japaneseFont, GameTimer* timer)
{
    //フォントとタイマーが有効か確認
    if (!font || !japaneseFont || !timer)
        return false;

    font_ = font;
    japaneseFont_ = japaneseFont;
    timer_ = timer;
    buttonSeHandle_ = Audio::Load("Audio/Game01Se04.wav", false, 3);
    winnerSeHandle_ = Audio::Load("Audio/Game01Se00.wav");
    winnerFollowupSeHandle_ = Audio::Load("Audio/Game01Se02.wav");
    falseStartSeHandle_ = Audio::Load("Audio/Game01Se06.wav");
    countdownSeHandle_ = Audio::Load("Audio/Game01Se07.wav");
    SetScreenSize(screenWidth, screenHeight);

    startButton_.reset(new GuiButton(0, 0, 180, 56, "START"));
    leftStopButton_.reset(new GuiButton(0, 0, 180, 56, "STOP"));
    rightStopButton_.reset(new GuiButton(0, 0, 180, 56, "STOP"));

    startButton_->SetDescription("CountStart");
    leftStopButton_->SetDescription("L Stop");
    rightStopButton_->SetDescription("R Stop");

    startButton_->onClick = [this]() { OnStartClicked(); };
    leftStopButton_->onClick = [this]() { OnStopClicked(-1); };
    rightStopButton_->onClick = [this]() { OnStopClicked(1); };

    UpdateButtonPositions();//
    Reset();
    return true;
}

void PlaySceneButton::SetScreenSize(float screenWidth, float screenHeight)
{
    screenWidth_ = (std::max)(1.0f, screenWidth);
    screenHeight_ = (std::max)(1.0f, screenHeight);
    UpdateButtonPositions();
}

void PlaySceneButton::UpdateButtonPositions()
{
    const int buttonWidth = 180;
    const int buttonHeight = 56;
    const int bottomMargin = 24;
    const int y = (std::max)(0, static_cast<int>(screenHeight_) - buttonHeight - bottomMargin);

    if (startButton_)
    {
        startButton_->SetPosition((static_cast<int>(screenWidth_) - buttonWidth) / 2, y);
        startButton_->SetSize(buttonWidth, buttonHeight);
    }
    if (leftStopButton_)
    {
        leftStopButton_->SetPosition(24, y);
        leftStopButton_->SetSize(buttonWidth, buttonHeight);
    }
    if (rightStopButton_)
    {
        rightStopButton_->SetPosition(
            (std::max)(0, static_cast<int>(screenWidth_) - buttonWidth - 24), y);
        rightStopButton_->SetSize(buttonWidth, buttonHeight);
    }
}

void PlaySceneButton::Reset()
{
    if (timer_)
        timer_->Reset();

    leftPressed_ = false;
    rightPressed_ = false;
    hasResult_ = false;
    gameFinished_ = false;
    leftTime_ = 0.0;
    rightTime_ = 0.0;
    leftFalseStarts_ = 0;
    rightFalseStarts_ = 0;
    winnerSide_ = 0;
    resultText_.clear();
    winnerFollowupPending_ = false;

    //フライング表示もリセット
    falseStartPending_ = false;

    if (startButton_) startButton_->SetActive(true);
    if (leftStopButton_) leftStopButton_->SetActive(false);
    if (rightStopButton_) rightStopButton_->SetActive(false);
}

void PlaySceneButton::OnStartClicked()
{
    if (!timer_ || timer_->IsWaiting() || timer_->IsRunning())
        return;

    if (startButton_ && startButton_->IsActive())
        Audio::Play(buttonSeHandle_);

    //新しいゲーム開始時はフライング回数をリセット
    leftFalseStarts_ = 0;
    rightFalseStarts_ = 0;
    leftPressed_ = false;
    rightPressed_ = false;
    hasResult_ = false;
    gameFinished_ = false;
    leftTime_ = 0.0;
    rightTime_ = 0.0;
    winnerSide_ = 0;
    resultText_.clear();

    static std::mt19937 engine((std::random_device())());
    std::uniform_int_distribution<int> distribution(10, 20);

    timer_->Start(distribution(engine));
    countdownStartTime_ = std::chrono::steady_clock::now();

    startButton_->SetActive(false);
    leftStopButton_->SetActive(true);
    rightStopButton_->SetActive(true);
}

void PlaySceneButton::ResetRound(bool restartTimer)
{
    leftPressed_ = false;
    rightPressed_ = false;
    leftTime_ = 0.0;
    rightTime_ = 0.0;
    hasResult_ = false;
    gameFinished_ = false;
    winnerSide_ = 0;
    resultText_.clear();

    if (leftStopButton_) leftStopButton_->SetActive(true);
    if (rightStopButton_) rightStopButton_->SetActive(true);
    if (startButton_) startButton_->SetActive(false);

    //
    if (restartTimer && timer_)
    {
        timer_->Start(timer_->GetTargetSeconds());
        countdownStartTime_ = std::chrono::steady_clock::now();
    }
}

void PlaySceneButton::OnStopClicked(int side)
{
    GuiButton* pressedButton = (side < 0) ? leftStopButton_.get() : rightStopButton_.get();
    if (!pressedButton || !pressedButton->IsActive())
        return;

    Audio::Play(buttonSeHandle_);

    if (!timer_ || !timer_->IsRunning() || gameFinished_)
        return;

    const double elapsed = timer_->GetElapsedSeconds();
    const double target = static_cast<double>(timer_->GetTargetSeconds());

    //ターゲット時間より前ならフライング。Xを加算して両者とも再挑戦。
    if (elapsed < target)
    {
        Audio::Play(falseStartSeHandle_);

        if (side < 0)
            ++leftFalseStarts_;
        else
            ++rightFalseStarts_;

        const int falseStartCount = (side < 0) ? leftFalseStarts_ : rightFalseStarts_;
        if (falseStartCount >= 3)
        {
            //3回目のフライングで、そのプレイヤーの負けとしてラウンド終了。
            hasResult_ = true;
            gameFinished_ = true;
            winnerSide_ = (side < 0) ? 1 : -1;
            resultText_ = (side < 0) ? "L LOSE - R WIN" : "R LOSE - L WIN";

            timer_->Stop();
            if (startButton_) startButton_->SetActive(true);
            if (leftStopButton_) leftStopButton_->SetActive(false);
            if (rightStopButton_) rightStopButton_->SetActive(false);
            return;
        }

        //フライング表示後、n秒待って仕切り直す
        timer_->Stop();

        falseStartPending_ = true;
        falseStartMessageEndTime_ =
            std::chrono::steady_clock::now() + std::chrono::seconds(3);//<-まつ秒すう

        //操作を無効化
        leftStopButton_->SetActive(false);
        rightStopButton_->SetActive(false);

        return;
    }

    //このプレイヤーの押下時刻を記録。タイマー自体は止めない。
    if (side < 0)
    {
        if (leftPressed_)
            return;

        leftPressed_ = true;
        leftTime_ = elapsed;
        leftStopButton_->SetActive(false);
    }
    else
    {
        if (rightPressed_)
            return;

        rightPressed_ = true;
        rightTime_ = elapsed;
        rightStopButton_->SetActive(false);
    }

    //L/R両方の押下が揃った時点で勝敗判定
    if (leftPressed_ && rightPressed_)
    {
        hasResult_ = true;
        gameFinished_ = true;

        const double leftDifference = std::abs(leftTime_ - target);
        const double rightDifference = std::abs(rightTime_ - target);

        if (leftDifference < rightDifference)
        {
            winnerSide_ = -1;
            resultText_ = "L WIN";
        }
        else if (rightDifference < leftDifference)
        {
            winnerSide_ = 1;
            resultText_ = "R WIN";
        }
        else
        {
            winnerSide_ = 0;
            resultText_ = "DRAW";
        }

        if (winnerSide_ != 0)
        {
            Audio::Play(winnerSeHandle_);
            winnerFollowupPending_ = true;
            winnerFollowupTime_ = std::chrono::steady_clock::now()
                + std::chrono::milliseconds(500);
        }

        //2人とも押したのでタイマーを止める（押下時刻は個別に記録済み）。
        timer_->Stop();

        //リザルト画面へ
        //SceneManager::ChangeScene();
        startButton_->SetActive(true);
    }
}

void PlaySceneButton::Draw()
{
    controllerTest_.Draw();

    if (!font_ || !timer_)
        return;

    //フライング表示が終了したらラウンドを仕切り直す
    if (falseStartPending_
        && std::chrono::steady_clock::now() >= falseStartMessageEndTime_)
    {
        falseStartPending_ = false;
        ResetRound(true);
    }

    //コントローラーのAボタン入力
    const bool pad0A = Input::IsPadButtonDown(XINPUT_GAMEPAD_A, 0);
    const bool pad1A = Input::IsPadButtonDown(XINPUT_GAMEPAD_A, 1);

    //START可能な状態か確認
    const bool canStart = !timer_->IsWaiting() && !timer_->IsRunning();

    //フライング表示中は入力を受け付けない
    if (!falseStartPending_)
    {
        if (canStart && (pad0A || pad1A))
        {
            //どちらかのAボタンを押したらゲーム開始
            OnStartClicked();
        }
        else
        {
            //ゲーム中は各プレイヤーのSTOPとして扱う
            if (pad0A)
            {
                OnStopClicked(-1);
            }

            if (pad1A)
            {
                OnStopClicked(1);
            }
        }
    }

    if (!ImGui::GetCurrentContext())
        return;

    if (winnerFollowupPending_
        && std::chrono::steady_clock::now() >= winnerFollowupTime_)
    {
        Audio::Play(winnerFollowupSeHandle_);
        winnerFollowupPending_ = false;
    }

    const bool wasWaitingForCountdown = timer_->IsWaiting();
    timer_->Update();
    if (wasWaitingForCountdown && timer_->IsRunning())
        Audio::Play(countdownSeHandle_);

    if (startButton_) startButton_->Draw();
    if (leftStopButton_) leftStopButton_->Draw();
    if (rightStopButton_) rightStopButton_->Draw();

    char buffer[64]{};
    if (timer_->GetTargetSeconds() > 0)
    {
        std::snprintf(buffer, sizeof(buffer), "Target: %d", timer_->GetTargetSeconds());
        font_->Draw(screenWidth_ - 190.0f, 20.0f, buffer, 0xFFFFFFFF);
    }

    //待機時間にゲーム説明
    if (timer_->IsWaiting())
    {
        //待機開始からの経過時間
        const double elapsed =
            std::chrono::duration<double>(
                std::chrono::steady_clock::now() - countdownStartTime_
            ).count();

        //最初のFADE_START秒は不透明、最後のFADE_END秒でフェードアウト
        constexpr double FADE_START = 3.0;
        constexpr double FADE_END = 5.0;

        double opacity = 1.0;

        if (elapsed > FADE_START)
        {
            opacity = (std::max)(
                0.0,
                1.0 - (elapsed - FADE_START) / (FADE_END - FADE_START)
                );
        }

        const unsigned int alpha =
            static_cast<unsigned int>(opacity * 255.0);

        const unsigned int messageColor =
            (alpha << 24) | 0x00FFFFFF;

        //ターゲット時間
        std::snprintf(buffer, sizeof(buffer), "Target: %d second", timer_->GetTargetSeconds());

        japaneseFont_->Draw(screenWidth_ * 0.4f - 100.0f, screenHeight_ * 0.5f - 40.0f, buffer, messageColor);

        japaneseFont_->Draw(screenWidth_ * 0.3f - 130.0f, screenHeight_ * 0.5f + 10.0f, "Click[STOP] or XBOX(A)", messageColor);
    }
    else if (timer_->IsRunning() && timer_->GetTargetSeconds() > 0)
    {
        const double elapsed = timer_->GetElapsedSeconds();
        constexpr double FADE_SECONDS = 5.0;//この秒数まで徐々に透明化していく
        const double opacity = (std::max)(0.0, 1.0 - elapsed / FADE_SECONDS);
        const unsigned int alpha = static_cast<unsigned int>(opacity * 255.0);
        const unsigned int timerColor = (alpha << 24) | 0x00FFFFFF;

        std::snprintf(buffer, sizeof(buffer), "%05.2f", elapsed);
        font_->Draw(screenWidth_ * 0.5f - 45.0f, 20.0f, buffer, timerColor);
    }

    //フライング表示
    if (falseStartPending_)
    {
        font_->Draw(
            screenWidth_ * 0.5f - 130.0f,
            screenHeight_ * 0.5f - 30.0f,
            "TOO EARLY!",
            0xFFFF5555
        );
    }

    //各ボタンの下にフライング回数をXで表示
    const float resultY = screenHeight_ - 56.0f - 24.0f - 36.0f;
    std::string leftMarks;
    std::string rightMarks;
    for (int i = 0; i < leftFalseStarts_; ++i) leftMarks += "X ";
    for (int i = 0; i < rightFalseStarts_; ++i) rightMarks += "X ";

    if (!leftMarks.empty())
        font_->Draw(24.0f + 48.0f, resultY + 28.0f, leftMarks.c_str(), 0xFFFF5555);
    if (!rightMarks.empty())
        font_->Draw(screenWidth_ - 180.0f - 24.0f + 48.0f,
            resultY + 28.0f, rightMarks.c_str(), 0xFFFF5555);

    //押した瞬間の時間を、それぞれのボタンの近くに表示
    if (leftPressed_)
    {
        std::snprintf(buffer, sizeof(buffer), "%.2f", leftTime_);
        font_->Draw(24.0f + 48.0f, resultY, buffer, 0xFFFFFFFF);
    }
    if (rightPressed_)
    {
        std::snprintf(buffer, sizeof(buffer), "%.2f", rightTime_);
        font_->Draw(screenWidth_ - 180.0f - 24.0f + 48.0f, resultY, buffer, 0xFFFFFFFF);
    }

    if (hasResult_)
        font_->Draw(screenWidth_ * 0.5f - 40.0f, resultY - 40.0f,
            resultText_.c_str(), 0xFFFFFFFF);
}
