#pragma once
#include "Engine/GameObject.h"
#include "Engine/FontText.hpp"
#include "Config.h"

// 銃のレティクルを管理するクラス
class Reticle : public GameObject
{
    FontText textReticle_;       //テキスト描画用のオブジェクト

    // レティクルの座標
    XMFLOAT3 position_[Config::PLAYER_NUM];
    XMFLOAT3 rotate_;   // 回転
    XMFLOAT3 scale_;    // 拡大率

    // 画像読み込み
    int hImage_[Config::PLAYER_NUM];

    // 標準移動
    void Move();

    // 発射(二人分)
    void Shoot(int player);

    bool triggerOld_[Config::PLAYER_NUM] = { false, false };

public:
    // コンストラクタ
    Reticle(GameObject* parent);

    // デストラクタ
    ~Reticle();

    // 初期化
    void Initialize() override;

    // 更新
    void Update() override;

    // 描画
    void Draw() override;

    // 開放
    void Release() override;
};