#pragma once
// Engine
#include "GameObject.hpp"
#include "FontText.hpp"

// Game
#include "Config.hpp"

// 銃のレティクルを管理するクラス
class Reticle : public GameObject
{
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

private:
    //テキスト描画用のオブジェクト
    FontText textReticle_;

    // レティクルの座標
    XMFLOAT3 position_[Config::PLAYER_NUM]; // 位置
    XMFLOAT3 rotate_[Config::PLAYER_NUM];   // 回転
    XMFLOAT3 scale_[Config::PLAYER_NUM];    // 拡大率

    // 画像読み込み
    int hImage_[Config::PLAYER_NUM];
    // サウンド読み込み
    int hSound_;

    // 標準移動
    void Move();

    // 発射(二人分)
    void Shoot(int player);

    bool triggerOld_[Config::PLAYER_NUM] = {false, false};

};