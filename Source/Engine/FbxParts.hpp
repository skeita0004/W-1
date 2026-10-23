#pragma once
#include <vector>
#include <span>

#include <fbxsdk.h>
#include <d3d11.h>
#include <DirectXMath.h>

#include "Vertex.hpp"
#include "Material.hpp"
#include "Texture.hpp"
#include "Transform.hpp"

using namespace DirectX;

class Fbx;
struct SkinPartInstance;

struct RayCastData;

/// @brief 
class FbxParts
{
public:
    // ボーン構造体（関節情報）
    struct  Bone
    {
        XMMATRIX  bindPose;      // 初期ポーズ時のボーン変換行列
        //XMMATRIX  newPose;       // アニメーションで変化したときのボーン変換行列
        //XMMATRIX  diffPose;      // mBindPose に対する mNowPose の変化量
    };

    // ウェイト構造体（ボーンと頂点の関連付け）
    struct Weight
    {
        XMFLOAT3	posOrigin;		// 元々の頂点座標
        XMFLOAT3	normalOrigin;	// 元々の法線ベクトル
        int* pBoneIndex;		// 関連するボーンのID
        float* pBoneWeight;	// ボーンの重み
    };


public:
    FbxParts();
    ~FbxParts();

    const int GetMaterialCount()
    {
        return materialCount_;
    }
    ID3D11Buffer* GetVertexBuffer()
    {
        return pVertexBuffer_;
    }
    std::span<ID3D11Buffer*> GetIndexBuffer()
    {
        return pIndexBuffers_;
    }
    std::vector<Material>& GetMaterialList()
    {
        return materialList_;
    }
    const std::span<int32_t> GetIndexCountArray()
    {
        return std::span{indexCount_};
    }

    //FBXファイルから情報をロードして諸々準備する
    //引数：pNode　情報が入っているノード
    //戻値：結果
    HRESULT Init(FbxNode* pNode);

    void CreateSkinInstance(SkinPartInstance& _inst) const;
    void Skinning(FbxTime _time, SkinPartInstance& _inst) const;

    //任意のボーンの位置を取得
    //引数：boneName	取得したいボーンの位置
    //引数：position	ワールド座標での位置【out】
    //戻値：見つかればtrue
    bool GetBonePosition(std::string boneName, XMFLOAT3* position);

    //スキンメッシュ情報を取得
    //戻値：スキンメッシュ情報
    FbxSkin* GetSkinInfo()
    {
        return pSkinInfo_;
    }

    //レイキャスト（レイを飛ばして当たり判定）
    //引数：data	必要なものをまとめたデータ
    void RayCast(RayCastData* data);

private:
    //各データの個数
    int32_t              vertexCount_;        // 頂点数
    int32_t              polygonCount_;       // ポリゴン数
    std::vector<int32_t> indexCount_;         // インデックス数
    int32_t              materialCount_;      // マテリアルの個数
    int32_t              polygonVertexCount_; // ポリゴン頂点インデックス数 

    Vertex* pVertexData_;

    std::vector<std::vector<int>> ppIndexData_;


    //【頂点バッファ】
    //各頂点の情報（位置とか色とか）を格納するところ
    //頂点数分の配列にして使う
    ID3D11Buffer* pVertexBuffer_;

    //【インデックスバッファ】
    //「どの頂点」と「どの頂点」と「どの頂点」で３角形ポリゴンになるかの情報を格納するところ
    std::vector<ID3D11Buffer*> pIndexBuffers_;

    std::vector<Material> materialList_;

    // ボーン制御情報
    FbxSkin* pSkinInfo_;		// スキンメッシュ情報（スキンメッシュアニメーションのデータ本体）
    FbxCluster** ppCluster_;		// クラスタ情報（関節ごとに関連付けられた頂点情報）
    int				numBone_;		// FBXに含まれている関節の数
    Bone* pBoneArray_;	// 各関節の情報
    Weight* pWeightArray_;	// ウェイト情報（頂点の対する各関節の影響度合い）

    /////////privateな関数（Init関数から呼ばれる）//////////////////////////
    void InitVertex(fbxsdk::FbxMesh* mesh);	//頂点バッファ準備
    void InitMaterial(fbxsdk::FbxNode* pNode);	//マテリアル準備
    void InitTexture(fbxsdk::FbxSurfaceMaterial* pMaterial, const int32_t& i);	//テクスチャ準備(InitMaterial内から呼び出される)
    void InitIndex(fbxsdk::FbxMesh* mesh);		//インデックスバッファ準備
    void InitSkelton(FbxMesh* pMesh);			//骨の情報を準備
};

