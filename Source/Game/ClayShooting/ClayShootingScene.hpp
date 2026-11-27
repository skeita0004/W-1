#pragma once
// Engine
#include "GameObject.hpp"
#include "FontText.hpp"

// Game
#include "Config.hpp"

//プレイシーンを管理するクラス
class ClayShootingScene : public GameObject
{
public:
    //コンストラクタ
    //引数：parent  親オブジェクト（SceneManager）
    ClayShootingScene(GameObject* parent);

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;

private:
    // 背景のモデルタイプ
    enum BDModel
    {
        Ground,      // 地面
        Model_Max    // モデルの限界値
    };

    // サウンドの種類
    enum SoundType
    {
        Wind,      // 風
        Sound_Max  // サウンドの限界値
    };

    // テキスト描画用のオブジェクト
    FontText textPlay_;

    // 背景モデルの座標
    XMFLOAT3 position_[Model_Max]; // 位置
    XMFLOAT3 rotate_[Model_Max];   // 回転
    XMFLOAT3 scale_[Model_Max];    // 拡大率

    // モデル番号
    int hModel_[Model_Max]; 
    // サウンド読み込み
    int hSound_[Sound_Max];
    // 背景の画像読み込み
    int hBackGround_;
};