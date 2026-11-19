#include "ControllerTest.h"

#include "Engine/Input.h"

#include <imgui.h>

//振動の強さ
namespace
{
    constexpr int VIBRATION_POWER = 40000;

    //振動時間
    constexpr float VIBRATION_TIME = 0.35f;

    //振動と振動の間隔
    constexpr float VIBRATION_INTERVAL = 0.25f;
}


//振動を停止
void ControllerTest::StopAll()
{
    Input::SetPadVibration(0, 0, 0);
    Input::SetPadVibration(0, 0, 1);

    targetPad_ = -1;
}


//LまたはRを1回振動
void ControllerTest::StartSingle(int padID)
{
    StopAll();

    mode_ = VibrationMode::Single;

    targetPad_ = padID;
    timer_ = VIBRATION_TIME;

    Input::SetPadVibration(
        VIBRATION_POWER,
        VIBRATION_POWER,
        padID
    );
}


//W-Check開始
void ControllerTest::StartIdentify()
{
    StopAll();

    mode_ = VibrationMode::Identify;
    step_ = 0;

    StartIdentifyStep();
}


//W-Checkの各段階
void ControllerTest::StartIdentifyStep()
{
    //まず両方の振動を停止
    StopAll();

    //5段階終了したら完了
    if (step_ >= 5)
    {
        mode_ = VibrationMode::None;
        timer_ = 0.0f;
        return;
    }

    //0: L振動
    //1: 間隔
    //2: R振動
    //3: 間隔
    //4: R振動

    if (step_ == 0)
    {
        targetPad_ = 0;

        Input::SetPadVibration(
            VIBRATION_POWER,
            VIBRATION_POWER,
            0
        );

        timer_ = VIBRATION_TIME;
    }
    else if (step_ == 1 || step_ == 3)
    {
        //無振動の時間
        timer_ = VIBRATION_INTERVAL;
    }
    else
    {
        targetPad_ = 1;

        Input::SetPadVibration(
            VIBRATION_POWER,
            VIBRATION_POWER,
            1
        );
        
        timer_ = VIBRATION_TIME;
    }
}


//振動状態を更新
void ControllerTest::UpdateVibration(float deltaTime)
{
    if (mode_ == VibrationMode::None)
    {
        return;
    }

    timer_ -= deltaTime;

    if (timer_ > 0.0f)
    {
        return;
    }

    //単体振動の場合は終了
    if (mode_ == VibrationMode::Single)
    {
        StopAll();
        mode_ = VibrationMode::None;
        return;
    }

    //W-Checkの場合は次の段階へ
    if (mode_ == VibrationMode::Identify)
    {
        step_++;
        StartIdentifyStep();
    }
}


//ImGui描画
void ControllerTest::Draw()
{
    if (!ImGui::GetCurrentContext())
    {
        return;
    }

    //振動処理
    UpdateVibration(ImGui::GetIO().DeltaTime);

    if (ImGui::Begin("ControllerTest"))
    {
        ImGui::Text("Controller Assignment");
        ImGui::Separator();

        ImGui::Text("L : XInput Pad 0");
        ImGui::Text("R : XInput Pad 1");

        ImGui::Spacing();

        if (ImGui::Button("L-Check"))
        {
            StartSingle(0);
        }

        ImGui::SameLine();

        if (ImGui::Button("R-Check"))
        {
            StartSingle(1);
        }

        ImGui::Spacing();

        if (ImGui::Button("W-Check"))
        {
            StartIdentify();
        }

        ImGui::Separator();

        if (mode_ == VibrationMode::None)
        {
            ImGui::Text("Status : Ready");
        }
        else if (mode_ == VibrationMode::Single)
        {
            ImGui::Text("Status : Pad %d vibrating", targetPad_);
        }
        else if (mode_ == VibrationMode::Identify)
        {
            ImGui::Text("Status : Identifying...");
        }
    }

    ImGui::End();
}