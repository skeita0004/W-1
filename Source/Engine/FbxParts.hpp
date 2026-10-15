#pragma once
#include <fbxsdk.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include "Texture.hpp"
#include "Transform.hpp"

using namespace DirectX;

class Fbx;
struct RayCastData;

//-----------------------------------------------------------
//FBXの１つのパーツを扱うクラス
//-----------------------------------------------------------
class FbxParts
{
public:
	// 一つの頂点情報を格納する構造体
	struct VERTEX
	{
		XMFLOAT3 position;
		XMFLOAT3 normal;
		XMFLOAT3 uv;
	};

	//【コンスタントバッファー】
	// GPU(シェーダ側)へ送る数値をまとめた構造体
	//Simple3D.hlslのグローバル変数と対応させる
	struct CONSTANT_BUFFER
	{
		XMMATRIX worldVewProj;	//ワールド、ビュー、プロジェクション行列の合成（頂点変換に使用）
		XMMATRIX normalTrans;	//回転行列と拡大行列の逆行列を合成したもの（法線の変形に使用）
		XMMATRIX world;			//ワールド行列
		XMFLOAT4 lightDirection;//ライトの向き
		XMFLOAT4 diffuse;		//ディフューズカラー。マテリアルの色。（テクスチャ貼ってるときは使わない）
		XMFLOAT4 ambient;		//アンビエント
		XMFLOAT4 speculer;		//スペキュラー（Lambertの場合は0）
		XMFLOAT4 cameraPosition;//カメラの位置（ハイライトの計算に必要）
		FLOAT	 shininess;		//ハイライトの強さ（MayaのCosinePower）
		BOOL	 isTexture;		//テクスチャの有無
	};

	// マテリアル情報（質感の情報）
	struct  MATERIAL
	{
		DWORD		polygonCount;		//マテリアルのポリゴン数
		XMFLOAT4	diffuse;			//拡散反射光（ディフューズ）への反射強度
		XMFLOAT4	ambient;			//環境光（アンビエント）への反射強度
		XMFLOAT4	specular;			//鏡面反射光（スペキュラ）への反射強度
		float		shininess;			//ハイライトの強さ（サイズ）
		Texture*	pTexture;			//テクスチャ
	}*pMaterial_;

	// ボーン構造体（関節情報）
	struct  Bone
	{
		XMMATRIX  bindPose;      // 初期ポーズ時のボーン変換行列
		XMMATRIX  newPose;       // アニメーションで変化したときのボーン変換行列
		XMMATRIX  diffPose;      // mBindPose に対する mNowPose の変化量
	};

	// ウェイト構造体（ボーンと頂点の関連付け）
	struct Weight
	{
		XMFLOAT3	posOrigin;		// 元々の頂点座標
		XMFLOAT3	normalOrigin;	// 元々の法線ベクトル
		int*		pBoneIndex;		// 関連するボーンのID
		float*		pBoneWeight;	// ボーンの重み
	};



	//各データの個数
	DWORD vertexCount_;		//頂点数
	DWORD polygonCount_;		//ポリゴ数
	DWORD indexCount_;		//インデックス数
	DWORD materialCount_;		//マテリアルの個数
	DWORD polygonVertexCount_;//ポリゴン頂点インデックス数 

	
	VERTEX *pVertexData_;
	DWORD** ppIndexData_;


	//【頂点バッファ】
	//各頂点の情報（位置とか色とか）を格納するところ
	//頂点数分の配列にして使う
	ID3D11Buffer *pVertexBuffer_;

	//【インデックスバッファ】
	//「どの頂点」と「どの頂点」と「どの頂点」で３角形ポリゴンになるかの情報を格納するところ
	ID3D11Buffer **ppIndexBuffer_ = NULL;

	//【定数バッファ】
	//シェーダー（Simple3D.hlsl）のグローバル変数に値を渡すためのもの
	ID3D11Buffer *pConstantBuffer_;


	// ボーン制御情報
	FbxSkin*		pSkinInfo_;		// スキンメッシュ情報（スキンメッシュアニメーションのデータ本体）
	FbxCluster**	ppCluster_;		// クラスタ情報（関節ごとに関連付けられた頂点情報）
	int				numBone_;		// FBXに含まれている関節の数
	Bone*			pBoneArray_;	// 各関節の情報
	Weight*			pWeightArray_;	// ウェイト情報（頂点の対する各関節の影響度合い）

	/////////privateな関数（Init関数から呼ばれる）//////////////////////////
	void InitVertex(fbxsdk::FbxMesh * mesh);	//頂点バッファ準備
	void InitMaterial(fbxsdk::FbxNode * pNode);	//マテリアル準備
	void InitTexture(fbxsdk::FbxSurfaceMaterial * pMaterial, const DWORD &i);	//テクスチャ準備
	void InitIndex(fbxsdk::FbxMesh * mesh);		//インデックスバッファ準備
	void InitSkelton(FbxMesh * pMesh);			//骨の情報を準備
	void IntConstantBuffer();	//コンスタントバッファ（シェーダーに情報を送るやつ）準備

public:
	FbxParts();
	~FbxParts();

	//FBXファイルから情報をロードして諸々準備する
	//引数：pNode　情報が入っているノード
	//戻値：結果
	HRESULT Init(FbxNode * pNode);


	//描画
	//引数：world	ワールド行列
	void Draw(Transform& transform);

	//ボーン有りのモデルを描画
	//引数：transform	行列情報
	//引数：time		フレーム情報（１アニメーション内の今どこか）
	void DrawSkinAnime(Transform& transform, FbxTime time);

	//ボーン無しのモデルを描画
	//引数：transform	行列情報
	//引数：time		フレーム情報（１アニメーション内の今どこか）
	//引数：scene		Fbxファイルから読み込んだシーン情報
	void DrawMeshAnime(Transform& transform, FbxTime time, FbxScene* scene);

	//任意のボーンの位置を取得
	//引数：boneName	取得したいボーンの位置
	//引数：position	ワールド座標での位置【out】
	//戻値：見つかればtrue
	bool GetBonePosition(std::string boneName, XMFLOAT3	* position);

	//スキンメッシュ情報を取得
	//戻値：スキンメッシュ情報
	FbxSkin* GetSkinInfo() { return pSkinInfo_; }

	//レイキャスト（レイを飛ばして当たり判定）
	//引数：data	必要なものをまとめたデータ
	void RayCast(RayCastData *data);
};

