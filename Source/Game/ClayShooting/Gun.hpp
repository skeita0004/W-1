#pragma once
// Engine
#include "GameObject.hpp"
#include "FontText.hpp"

// Game
#include "Config.hpp"

//銃を管理するクラス
class Gun : public GameObject
{
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

private:
    //テキスト描画用のオブジェクト
    FontText textGun_;

    XMFLOAT3 position_[Config::PLAYER_NUM]; // 位置
    XMFLOAT3 rotate_[Config::PLAYER_NUM];   // 回転
    XMFLOAT3 scale_[Config::PLAYER_NUM];    // 拡大率

    int hModel_;        // モデル番号
    //int hImage_;      // 画像読み込み

    //移動
    void Move();

};