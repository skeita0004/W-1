#pragma once
#pragma once

class ControllerTest
{
public:
    void Draw();

private:
    enum class VibrationMode
    {
        None,
        Single,
        Identify
    };

    VibrationMode mode_ = VibrationMode::None;

    //振動対象のコントローラー
    int targetPad_ = -1;

    //振動残り時間
    float timer_ = 0.0f;

    //W-Checkの現在の段階
    int step_ = 0;

    //単体振動を開始
    void StartSingle(int padID);

    //L/R識別用の振動を開始
    void StartIdentify();

    //W-Checkの各段階を開始
    void StartIdentifyStep();

    //振動状態を更新
    void UpdateVibration(float deltaTime);

    //振動を停止
    void StopAll();
};