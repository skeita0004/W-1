#pragma once
#include "GameObject.hpp"
#include "FontText.hpp"
#include "GuiButton.hpp"
#include "01GameTimer.hpp"
#include "01PlaySceneButton.hpp"
#include "Game01Player.hpp"

//プレイシーンを管理するクラス
class QuickDrawScene : public GameObject
{
	//const int ENEMY_NUM;	//敵の数
	FontText text_;		//テキスト描画用のオブジェクト
	FontText japaneseText_;	//日本語テキスト描画用のオブジェクト
	GameTimer timer_;
	int bgmHandle_ = -1;
	PlaySceneButton playSceneButton_;

public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	QuickDrawScene(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

private:
	GuiButton* startButton = nullptr;
	Game01Player* leftPlayer_ = nullptr;
	Game01Player* rightPlayer_ = nullptr;
};