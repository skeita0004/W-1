#pragma once
// Engine
#include "../Engine/GameObject.hpp"
#include "../Engine/FontText.hpp"

// Game
#include "Config.hpp"

class Clay;

// UIを管理するクラス
class UI : public GameObject
{
public:
    // コンストラクタ
    UI(GameObject* parent);

    // デストラクタ
    ~UI();

    // 初期化
    void Initialize() override;

    // 更新
    void Update() override;

    // 描画
    void Draw() override;

    // 開放
    void Release() override;

private:
    // UIの画像タイプ
    enum UIModel
    {
        Bullet_UI1p,  // 弾の画像(1P用)
        Bullet_UI2p,  // 弾の画像(2P用)
        Board_1p,     // 木板の画像(1P用)
        Board_2p,     // 木板の画像(2P用)
        UI_Max        // UIの限界値
    };

    //　テキスト描画用のオブジェクト
    FontText textB_num_;
    FontText textScore_;
    FontText textTimer_;

    // UIの座標
    XMFLOAT3 position_[UI_Max]; // 位置
    XMFLOAT3 rotate_[UI_Max];   // 回転
    XMFLOAT3 scale_[UI_Max];    // 拡大率

    // 画像読み込み
    int hImage_[UI_Max];
    // サウンド読み込み
    int hSound_;
    // 残弾数
    int Bullet_num[Config::PLAYER_NUM];
    // 試合時間
    float Game_Timer;

    bool triggerOld_[Config::PLAYER_NUM] = {false,false};

    // クレーのポインタを参照
    Clay* sc = nullptr;

};