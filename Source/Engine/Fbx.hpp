#pragma once
#include <d3d11.h>
#include <fbxsdk.h>
#include <vector>
#include <string>

#include <span>

#include "Transform.hpp"
#include "RayCast.hpp"

class FbxParts;
class SkinInstance;

/// @brief FBXファイルを扱うクラス
/// @note　ほとんどの処理は各パーツごとにFbxPartsクラスで行う
class Fbx
{
public:
    Fbx();
    ~Fbx();

    //ロード
    //引数：fileName	ファイル名
    //戻値：成功したかどうか
    virtual HRESULT Load(const std::string& fileName);

    //描画
    //引数：World	ワールド行列
    void CreateSkinInstance(SkinInstance& _inst) const;
    void UpdateSkin(int _frame, SkinInstance& _inst) const;
    void Release();

    //任意のボーンの位置を取得
    //引数：boneName	取得したいボーンの位置
    //戻値：ボーンの位置
    XMFLOAT3 GetBonePosition(std::string boneName);

    //レイキャスト（レイを飛ばして当たり判定）
    //引数：data	必要なものをまとめたデータ
    void RayCast(RayCastData* data);

    std::vector<FbxParts*>& GetParts()
    {
        return parts_;
    }

private:
    //FbxPartクラスをフレンドクラスにする
    //FbxPartのprivateな関数にもアクセス可
    friend class FbxParts;

    //モデルの各パーツ（複数あるかも）
    std::vector<FbxParts*>	parts_;

    //FBXファイルを扱う機能の本体
    FbxManager* pFbxManager_;

    //FBXファイルのシーン（Mayaで作ったすべての物体）を扱う
    FbxScene* pFbxScene_;


    // アニメーションのフレームレート
    FbxTime::EMode	_frameRate;

    //アニメーション速度
    float			_animSpeed;

    //アニメーションの最初と最後のフレーム
    int _startFrame, _endFrame;

    //ノードの中身を調べる
    //引数：pNode		調べるノード
    //引数：pPartsList	パーツのリスト
    void CheckNode(FbxNode* pNode, std::vector<FbxParts*>* pPartsList);

};

