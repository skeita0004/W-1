#pragma once
// Engine
#include "GameObject.hpp"
#include "FontText.hpp"

// Game
#include "Config.hpp"

// クレーを管理するクラス
class Clay : public GameObject
{
public:
    //コンストラクタ
    Clay(GameObject* parent);

    //デストラクタ
    ~Clay();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;

    // レティクルの中心がクレーに重なっているかの確認処理
    int FindOverlap(const XMFLOAT2& reticleNdc) const;

    // 発砲時、レティクルが重なっているクレーがあれば壊す
    bool TryBreak(int player, const XMFLOAT2& reticleNdc);
    bool TryBreak_1P(const XMFLOAT2& reticleNdc) { return TryBreak(Config::PLAYER_1, reticleNdc); }
    bool TryBreak_2P(const XMFLOAT2& reticleNdc) { return TryBreak(Config::PLAYER_2, reticleNdc); }

    // 合計スコアを取得
    static int GetTotalScore_1P() { return TotalScore_[Config::PLAYER_1]; }
    static int GetTotalScore_2P() { return TotalScore_[Config::PLAYER_2]; }

    // 合計スコアをリセット
    static void ScoreReset()
    {
        for (int i = 0; i < Config::PLAYER_NUM; i++) TotalScore_[i] = 0;
    }

    // クレーの最大数
    static const int Clay_Max = 10;

private:
    // 飛行物のモデルタイプ
    enum ClayType
    {
        Clay_A,       // 皿A
        Clay_B,       // 皿B
        Clay_C,       // 皿C
        Bottle_A,     // 空き瓶A
        Bottle_B,     // 空き瓶B
        Bomb,         // 爆弾
        ClayType_Max  // クレーの限界値
    };

    // 飛行物の軌道パターン
    enum OrbitType
    {
        Right_Straight, // 右真直ぐ
        Left_Straight,  // 左真直ぐ
        Right_Curve,    // 右曲がり
        Left_Curve      // 左曲がり
    };

    // 速度タイプ
    enum SpeedType
    {
        Slow,     // 遅い
        Normal,   // 普通
        Fast      // 速い
    };

    // テキスト描画用のオブジェクト
    FontText textClay_;

    // クレーの座標
    XMFLOAT3 position_[Clay_Max]; // 位置
    XMFLOAT3 rotate_[Clay_Max];   // 回転
    XMFLOAT3 scale_[Clay_Max];    // 拡大率

    int hModel_[ClayType_Max];  // モデル番号
    int hSound_[ClayType_Max];  // サウンド読み込み
    int Score_[ClayType_Max];   // 各クレーごとにスコア

    bool OnDown_[Clay_Max]; // 下がるか上がるかのフラグ　    ※ true：下がる / false：上がる
    bool IsDead_[Clay_Max]; // クレーの死亡しているかフラグ  ※ true：死亡 / false：生存

    // 当たり判定の大きさ
    const float HitRange;

    // 合計のスコア
    static int TotalScore_[Config::PLAYER_NUM];

    //移動
    void Move();

    // 全てのクレーが壊れているか
    bool AllBroken() const;
};