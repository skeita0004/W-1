#include "FontText.hpp"
#include <imgui.h>

/*
フォントテキスト描画クラス
【使い方】
1.Initialize() でフォントを読み込む
2.Draw() で指定した座標に文字を描画する
3.不要になったら Release() を呼び出す

【使用例】
//フォントテキスト描画クラスのインスタンスを作成
FontText font;

//フォントの読み込み（Ryeフォント、サイズ42）
font.Initialize("Fonts/Rye-Regular.ttf", 42.0f);

//文字の描画
font.Draw(100.0f, 100.0f, "Hello World");

//解放
font.Release();

 【注意事項】

 ・Main.cppについて、
 　FontText::IInitialize() は ImGui::CreateContext() の実行後に呼び出すこと。
   ImGuiの初期化前はフォントを読み込めない。

 ・FontTextを使用するクラスより先に破棄しないこと。
   例えば、PlaySceneがFontTextを所有し、
   PlaySceneButtonがポインタで参照する場合、
   PlaySceneButtonが使用している間はFontTextを保持する。

 ・ImGui::DestroyContext() は FontText の使用終了後に呼び出すこと。
*/

HRESULT FontText::Initialize(const char* fileName, float fontSize)
{
	Release();
	if (!fileName || fileName[0] == '\0' || fontSize <= 0.0f || !ImGui::GetCurrentContext())
		return E_INVALIDARG;

	font_ = ImGui::GetIO().Fonts->AddFontFromFileTTF(
		fileName,
		fontSize,
		nullptr,
		ImGui::GetIO().Fonts->GetGlyphRangesJapanese());
	if (!font_)
		return E_FAIL;

	fontSize_ = fontSize;
	return S_OK;
}

void FontText::Draw(float x, float y, const char* text, unsigned int color) const
{
	if (!font_ || !text || !ImGui::GetCurrentContext())
		return;

	ImGui::GetForegroundDrawList()->AddText(
		font_, fontSize_, ImVec2(x, y), static_cast<ImU32>(color), text);
}

void FontText::Release()
{
	font_ = nullptr;
	fontSize_ = 24.0f;
}
