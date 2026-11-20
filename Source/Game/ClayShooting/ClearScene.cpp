#include "ClearScene.h"
#include "Engine/Image.h"
#include "Engine//FontText.hpp"
#include "Clay.h"

#include "../include/imgui.h"

// ↓こいつがないと文字が表示されない
namespace
{
    // PlaySceneなどで既にロード済みのフォント(Rye-Regular)を探す
    // 見つからなければImGuiのデフォルトフォントを返す
    ImFont* FindLoadedFont()
    {
        ImGuiIO& io = ImGui::GetIO();
        for (int i = 0; i < io.Fonts->Fonts.Size; i++)
        {
            ImFont* f = io.Fonts->Fonts[i];
            const char* name = f->GetDebugName();
            if (name && strstr(name, "Rye-Regular"))
            {
                return f;
            }
        }
        return ImGui::GetFont();
    }

    // 文字を描画する(フォント追加は一切しない)
    void DrawTextAt(float x, float y, float size, const char* text)
    {
        ImFont* font = FindLoadedFont();
        ImGui::GetBackgroundDrawList()->AddText(
            font, size, ImVec2(x, y), IM_COL32(255, 255, 255, 255), text);
    }
}

//コンストラクタ
ClearScene::ClearScene(GameObject * parent)
    : GameObject(parent, "ClearScene"), hImage_{ -1,-1,-1,-1 }
{
}

//初期化
void ClearScene::Initialize()
{
    //画像データのロード
    hImage_[0] = Image::Load("Image/BG.jpg");
    assert(hImage_[0] >= 0);
    hImage_[1] = Image::Load("Image/WIN.png");
    assert(hImage_[1] >= 0);
    hImage_[2] = Image::Load("Image/LOSE.png");
    assert(hImage_[2] >= 0);
    hImage_[3] = Image::Load("Image/UI_1.png");
    assert(hImage_[3] >= 0);

    // 一応あると楽だからここに
    Image::SetRect(hImage_[0], 0, 0, 1280, 720);

    position_[1] = { -0.25f, -0.3f, 0.0f }; // 位置(プレイヤー１)
    position_[2] = { 0.25f, -0.3f,  0.0f }; // 位置(プレイヤー２)
    position_[3] = { 0.0f, 0.0f,  0.0f }; // 後ろの枠

    scale_[1]    = {  0.5f, 0.5f, 0.0f }; // 拡大率（「WIN」,「LOSE」表示用）
    scale_[2]    = {  1.2f, 3.0f, 0.0f }; // 拡大率（後ろの枠表示用）

    //textP_num_.Initialize("Fonts/Rye-Regular.ttf", 72.0f);
    //textFinal_S_.Initialize("Fonts/Rye-Regular.ttf", 72.0f);
}

//更新
void ClearScene::Update()
{
}

//描画
void ClearScene::Draw()
{
    // 背景表示
    Image::Draw(hImage_[0]);

    // 結果発表の枠
    transform_.position_ = { position_[3].x,position_[3].y,position_[3].z };  // 位置
    transform_.scale_ = { scale_[2].x, scale_[2].y, scale_[2].z };          // 拡大率
    Image::SetTransform(hImage_[3], transform_);
    Image::Draw(hImage_[3]);

    // プレイヤー１の勝ち
    if (Clay::GetTotalScore_1P() > Clay::GetTotalScore_2P()) {
        // プレイヤー１に「WIN」表示
        transform_.position_ = { position_[1].x,position_[1].y,position_[1].z };  // 位置
        transform_.scale_ = { scale_[1].x, scale_[1].y, scale_[1].z };          // 拡大率
        Image::SetTransform(hImage_[1], transform_);
        Image::Draw(hImage_[1]);
        // プレイヤー２に「LOSE」表示
        transform_.position_ = { position_[2].x,position_[2].y,position_[2].z };  // 位置
        transform_.scale_ = { scale_[1].x, scale_[1].y, scale_[1].z };          // 拡大率
        Image::SetTransform(hImage_[2], transform_);
        Image::Draw(hImage_[2]);
    }
    // プレイヤー２の勝ち
    else if (Clay::GetTotalScore_1P() < Clay::GetTotalScore_2P()) {
        // プレイヤー１に「LOSE」表示
        transform_.position_ = { position_[1].x,position_[1].y,position_[1].z };  // 位置
        transform_.scale_ = { scale_[1].x, scale_[1].y, scale_[1].z };          // 拡大率
        Image::SetTransform(hImage_[2], transform_);
        Image::Draw(hImage_[2]);
        // プレイヤー２に「WIN」表示
        transform_.position_ = { position_[2].x,position_[2].y,position_[2].z };  // 位置
        transform_.scale_ = { scale_[1].x, scale_[1].y, scale_[1].z };          // 拡大率
        Image::SetTransform(hImage_[1], transform_);
        Image::Draw(hImage_[1]);
    }
    // 引き分け
    else {
        // プレイヤー１に「WIN」表示
        transform_.position_ = { position_[1].x,position_[1].y,position_[1].z };  // 位置
        transform_.scale_ = { scale_[1].x, scale_[1].y, scale_[1].z };          // 拡大率
        Image::SetTransform(hImage_[1], transform_);
        Image::Draw(hImage_[1]);
        // プレイヤー２に「WIN」表示
        transform_.position_ = { position_[2].x,position_[2].y,position_[2].z };  // 位置
        transform_.scale_ = { scale_[1].x, scale_[1].y, scale_[1].z };          // 拡大率
        Image::SetTransform(hImage_[1], transform_);
        Image::Draw(hImage_[1]);
    }

    // スコア表示
    {
        char str[64];
        sprintf_s(str, "%.5d", Clay::GetTotalScore_1P());
        //textFinal_S_.Draw(370, 280, str);
        DrawTextAt(370, 280, 72.0f, str);

        //char str[64];
        sprintf_s(str, "%.5d", Clay::GetTotalScore_2P());
        //textFinal_S_.Draw(700, 280, str);
        DrawTextAt(700, 280, 72.0f, str);
    }

    // textP_num_.Draw(360, 140, "Player1");
    // textP_num_.Draw(680, 140, "Player2");
    DrawTextAt(360, 140, 72.0f, "Player1");
    DrawTextAt(680, 140, 72.0f, "Player2");
}

//開放
void ClearScene::Release()
{
}