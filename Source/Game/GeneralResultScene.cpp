#include "GeneralResultScene.hpp"
#include "WinCount.hpp"

#include <imgui.h>
#include <cstdio>

//コンストラクタ
GeneralResultScene::GeneralResultScene(GameObject* parent)
    : GameObject(parent, "GeneralResultScene")
{
}

//初期化
void GeneralResultScene::Initialize()
{
    text_.Initialize("Fonts/Rye-Regular.ttf", 42.0f);
}

//更新
void GeneralResultScene::Update()
{
}

//描画
void GeneralResultScene::Draw()
{
    const ImVec2 size = ImGui::GetIO().DisplaySize;

    text_.Draw(
        size.x * 0.5f - 180.0f,
        60.0f,
        "General Result"
    );

    char buffer[128];

    //Player01
    text_.Draw(100.0f, 180.0f, "Player01");

    sprintf_s(buffer, "Win  : %d", WinCount::player01.win);
    text_.Draw(100.0f, 240.0f, buffer);

    sprintf_s(buffer, "Lose : %d", WinCount::player01.lose);
    text_.Draw(100.0f, 300.0f, buffer);

    sprintf_s(buffer, "Draw : %d", WinCount::player01.draw);
    text_.Draw(100.0f, 360.0f, buffer);

    //Player02
    text_.Draw(size.x * 0.5f + 100.0f, 180.0f, "Player02");

    sprintf_s(buffer, "Win  : %d", WinCount::player02.win);
    text_.Draw(size.x * 0.5f + 100.0f, 240.0f, buffer);

    sprintf_s(buffer, "Lose : %d", WinCount::player02.lose);
    text_.Draw(size.x * 0.5f + 100.0f, 300.0f, buffer);

    sprintf_s(buffer, "Draw : %d", WinCount::player02.draw);
    text_.Draw(size.x * 0.5f + 100.0f, 360.0f, buffer);
}

//開放
void GeneralResultScene::Release()
{
    text_.Release();
}