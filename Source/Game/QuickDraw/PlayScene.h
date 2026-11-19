#pragma once
#include "Engine/GameObject.h"
#include "Engine/FontText.hpp"
#include "Engine/GuiButton.h"
#include "Engine/GameTimer.h"
#include "PlaySceneButton.h"
#include "Game01Player.h"

//プレイシーンを管理するクラス
class PlayScene : public GameObject
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
	PlayScene(GameObject* parent);

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