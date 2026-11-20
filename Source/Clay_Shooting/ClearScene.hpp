#pragma once
#include "Engine/GameObject.h"
#include "Engine/FontText.hpp"

//class Clay;

//プレイシーンを管理するクラス
class ClearScene : public GameObject
{
    FontText textP_num_;     //テキスト描画用のオブジェクト
    FontText textFinal_S_;   //テキスト描画用のオブジェクト

    int hImage_[4];    //画像番号

    // UIの座標
    XMFLOAT3 position_[4]; // 位置
    XMFLOAT3 scale_[3];    // 拡大率

    //Clay* sc = nullptr;

public:
    //コンストラクタ
    //引数：parent  親オブジェクト（SceneManager）
    ClearScene(GameObject* parent);

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;
};