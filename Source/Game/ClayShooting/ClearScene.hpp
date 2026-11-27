#pragma once
// Engine
#include "GameObject.hpp"
#include "FontText.hpp"

//class Clay;

//プレイシーンを管理するクラス
class ClearScene : public GameObject
{
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

private:
    // 文字表示タイプ
    enum ClearImage
    {
        BG,        // 背景
        WIN,       // 「WIN」の表示
        LOSE,      // 「LOSE」の表示
        Board,     // 木板の画像
        CImage_Max // クリア画像の限界値
    };

    //テキスト描画用のオブジェクト
    FontText textP_num_;
    FontText textFinal_S_;

    // 文字表示の座標
    XMFLOAT3 position_[CImage_Max]; // 位置
    XMFLOAT3 rotate_[CImage_Max];   // 回転
    XMFLOAT3 scale_[CImage_Max];    // 拡大率

    //画像番号
    int hImage_[CImage_Max];

    //Clay* sc = nullptr;

};