#pragma once
#include "Engine/GameObject.h"
#include "Engine/FontText.hpp"

//プレイシーンを管理するクラス
class PlayScene : public GameObject
{
    //const int ENEMY_NUM;  //敵の数
    FontText textPlay_;       //テキスト描画用のオブジェクト

    int hBackGround_; // 背景

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
};