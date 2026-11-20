#pragma once

// ゲーム全体で共有する定数
namespace Config
{
    // ---- 画面 ----
    constexpr int SCREEN_WIDTH = 1280;   // 画面の横幅(px)
    constexpr int SCREEN_HEIGHT = 720;   // 画面の縦幅(px)

    // ---- プレイヤー ----
    constexpr int PLAYER_NUM = 2;   // プレイヤー人数
    constexpr int PLAYER_1 = 0;     // 配列の添字（1P）
    constexpr int PLAYER_2 = 1;     // 配列の添字（2P）

    // ---- 入力 ----
    // 各プレイヤーが発砲に使うマウスボタン（0：左クリック / 1：右クリック）
    constexpr int MOUSE_BUTTON[PLAYER_NUM] = { 0, 1 };
    // 右トリガーを「押した」と判定する閾値
    constexpr float TRIGGER_THRESHOLD = 0.5f;

    // ---- フォント ----
    constexpr const char* FONT_PATH = "Fonts/Rye-Regular.ttf";
    constexpr float FONT_SIZE_DEFAULT = 28.0f;

    // ---- 便利関数 ----
    // value を min～max の範囲に収める（C++14でも使えるよう自作）
    inline float Clamp(float value, float minValue, float maxValue)
    {
        return (value < minValue) ? minValue : ((value > maxValue) ? maxValue : value);
    }
}