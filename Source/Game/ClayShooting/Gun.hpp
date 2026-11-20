#pragma once
#include "Engine/GameObject.h"
#include "Engine/FontText.hpp"
#include "Config.h"

//銃を管理するクラス
class Gun : public GameObject
{
    FontText textGun_;       //テキスト描画用のオブジェクト

    XMFLOAT3 position_[Config::PLAYER_NUM]; // 位置
    XMFLOAT3 rotate_[Config::PLAYER_NUM];   // 回転
    XMFLOAT3 scale_[Config::PLAYER_NUM];    // 拡大率

    int hModel_;        // モデル番号
    //int hImage_;      // 画像読み込み

    //移動
    void Move();

public:
    //コンストラクタ
    Gun(GameObject* parent);

    //デストラクタ
    ~Gun();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;
};