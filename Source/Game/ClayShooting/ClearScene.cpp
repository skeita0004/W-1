// Engine
#include "Input.hpp"
#include "Image.hpp"
#include "Audio.hpp"
#include "SceneManager.hpp"
#include "CameraSet.hpp"
#include "FontText.hpp"

// Game
#include "ClearScene.hpp"
#include "Clay.hpp"

// その他
#include <DirectXMath.h>
#include "imgui.h"

using namespace DirectX;
using namespace Config;

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

    // ↓====== 定数部屋 ======↓
    const int str_num = 64;

    // 文字表示の初期値(一応あると楽だからここに)
    const XMFLOAT3 Init_Pos[4] // 位置
        = {{0.0f,0.0f,0.0f},{-0.25f,-0.3f,0.0f},{0.25f,-0.3f,0.0f},{0.0f,0.0f,0.0f}};
    const XMFLOAT3 Init_Rot[4] // 回転
        = {{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f}};
    const XMFLOAT3 Init_Sca[4] // 拡大率
        = {{1.0f,1.0f,0.0f},{0.5f,0.5f,0.0f},{0.5f,0.5f,0.0f},{1.2f,3.0f,0.0f}};
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
    {
        hImage_[BG]
            = Image::Load("Image/ClayShooting/BG.jpg");
        hImage_[WIN]
            = Image::Load("Image/ClayShooting/WIN.png");
        hImage_[LOSE]
            = Image::Load("Image/ClayShooting/LOSE.png");
        hImage_[Board]
            = Image::Load("Image/ClayShooting/UI_1.png");

        assert(hImage_[BG] >= 0);
        assert(hImage_[WIN] >= 0);
        assert(hImage_[LOSE] >= 0);
        assert(hImage_[Board] >= 0);
    }

    // 一応あると楽だからここに
    {
        Image::SetRect(hImage_[BG], 0, 0, 1280, 720);

        for (int i = 1; i < CImage_Max; i++)
        {
            position_[i] = Init_Pos[i];
            rotate_[i]   = Init_Rot[i];
            scale_[i]    = Init_Sca[i];
        }

        /*
        position_[1] = {-0.25f, -0.3f, 0.0f}; // 位置(プレイヤー１)
        position_[2] = {0.25f, -0.3f,  0.0f}; // 位置(プレイヤー２)
        position_[3] = {0.0f, 0.0f,  0.0f}; // 後ろの枠

        scale_[1] = {0.5f, 0.5f, 0.0f}; // 拡大率（「WIN」,「LOSE」表示用）
        scale_[2] = {1.2f, 3.0f, 0.0f}; // 拡大率（後ろの枠表示用）
        */
    }

    textP_num_.Initialize("Fonts/Rye-Regular.ttf", 72.0f);
    textFinal_S_.Initialize("Fonts/Rye-Regular.ttf", 72.0f);
}

//更新
void ClearScene::Update()
{
}

//描画
void ClearScene::Draw()
{
    // 背景表示
    Image::Draw(hImage_[BG]);

    // 結果発表の枠
    transform_.position = position_[Board]; // 位置
    transform_.scale    = scale_[Board];    // 拡大率
    Image::SetTransform(hImage_[Board], transform_);
    Image::Draw(hImage_[Board]);

    // プレイヤー１の勝ち
    if (Clay::GetTotalScore_1P() > Clay::GetTotalScore_2P()) {
        // プレイヤー１に「WIN」表示
        transform_.position = position_[WIN];  // 位置
        transform_.scale    = scale_[WIN];     // 拡大率
        Image::SetTransform(hImage_[WIN], transform_);
        Image::Draw(hImage_[WIN]);
        // プレイヤー２に「LOSE」表示
        transform_.position = position_[LOSE];  // 位置
        transform_.scale    = scale_[LOSE];     // 拡大率
        Image::SetTransform(hImage_[LOSE], transform_);
        Image::Draw(hImage_[LOSE]);
    }
    // プレイヤー２の勝ち
    else if (Clay::GetTotalScore_1P() < Clay::GetTotalScore_2P()) {
        // プレイヤー１に「LOSE」表示
        transform_.position = position_[WIN];  // 位置
        transform_.scale    = scale_[WIN];     // 拡大率
        Image::SetTransform(hImage_[LOSE], transform_);
        Image::Draw(hImage_[LOSE]);
        // プレイヤー２に「WIN」表示
        transform_.position = position_[LOSE];  // 位置
        transform_.scale    = scale_[LOSE];     // 拡大率
        Image::SetTransform(hImage_[WIN], transform_);
        Image::Draw(hImage_[WIN]);
    }
    // 引き分け
    else {
        // プレイヤー１に「WIN」表示
        transform_.position = position_[WIN];  // 位置
        transform_.scale    = scale_[WIN];     // 拡大率
        Image::SetTransform(hImage_[WIN], transform_);
        Image::Draw(hImage_[WIN]);
        // プレイヤー２に「WIN」表示
        transform_.position = position_[LOSE];  // 位置
        transform_.scale    = scale_[LOSE];     // 拡大率
        Image::SetTransform(hImage_[WIN], transform_);
        Image::Draw(hImage_[WIN]);
    }

    // スコア表示
    {
        char str[str_num];
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