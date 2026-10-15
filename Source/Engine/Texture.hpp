#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <wincodec.h>
#include <string>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "LibFbxSDK-MT.lib")
#pragma comment(lib, "LibXml2-MT.lib")
#pragma comment(lib, "zlib-MT.lib")
#pragma comment( lib, "WindowsCodecs.lib" )

using namespace DirectX;

//-----------------------------------------------------------
//テクスチャ関連を扱うクラス
//-----------------------------------------------------------
class Texture
{
	ID3D11SamplerState*			pSampleLinear_;		//テクスチャサンプラー（テクスチャの貼り方）
	ID3D11ShaderResourceView*	pTextureSRV_;		//シェーダーリソースビュー（テクスチャをシェーダーに送るためのもの）
	XMFLOAT3					size_;				//画像ファイルのサイズ

public:
	Texture();
	~Texture();

	//ロード
	//引数：fileName	画像ファイル名
	//戻値：成功/失敗
	HRESULT Load(std::string fileName);


	//各アクセス関数
	ID3D11SamplerState* GetSampler() { return pSampleLinear_; }	//サンプラーの取得
	ID3D11ShaderResourceView* GetSRV() { return pTextureSRV_; }	//シェーダーリソースビューの取得
	XMFLOAT3 GetSize() { return size_; }	//画像サイズの取得
};

