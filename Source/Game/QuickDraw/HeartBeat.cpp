#include "HeartBeat.hpp"
#include "Input.hpp"

#include <algorithm>

//開始
void HeartBeat::Start()
{
    Stop();

    active_ = true;
    elapsed_ = 0.0f;
    beatTimer_ = 0.0f;
    pulseTimer_ = 0.0f;

    beatInterval_ = 1.2f;
    pulseDuration_ = 0.15f;

    power_ = 35000;
    pulsing_ = false;
}

//停止
void HeartBeat::Stop()
{
    SetVibration(0);

    active_ = false;
    pulsing_ = false;
    pulseTimer_ = 0.0f;
}

//振動設定
void HeartBeat::SetVibration(int power)
{
    Input::SetPadVibration(0, power, 0);
    Input::SetPadVibration(0, power, 1);
}

//鼓動開始
void HeartBeat::Pulse()
{
    SetVibration(power_);

    pulsing_ = true;
    pulseTimer_ = pulseDuration_;
}

//更新
void HeartBeat::Update(float deltaTime)
{
    if (!active_)
        return;

    elapsed_ += deltaTime;

    //時間経過に伴い鼓動間隔を短くする
    beatInterval_ = (std::max)(0.45f, 1.2f - elapsed_ * 0.05f);

    //時間経過に伴い振動強度を上げる
    power_ = static_cast<int>(
        (std::min)(static_cast<float>(maxPower_),
                   15000.0f + elapsed_ * 4000.0f)
    );

    //現在の鼓動を終了
    if (pulsing_)
    {
        pulseTimer_ -= deltaTime;

        if (pulseTimer_ <= 0.0f)
        {
            SetVibration(0);
            pulsing_ = false;
        }
    }

    //次の鼓動
    beatTimer_ -= deltaTime;

    if (beatTimer_ <= 0.0f && !pulsing_)
    {
        Pulse();
        beatTimer_ = beatInterval_;
    }
}