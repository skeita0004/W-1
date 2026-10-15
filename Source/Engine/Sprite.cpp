#include "Sprite.hpp"
#include "Direct3D.hpp"
#include "SafeCleaning.hpp"

//コンストラクタ
Sprite::Sprite():
	pTexture_(nullptr)
{
}

//デストラクタ
Sprite::~Sprite()
{
    using namespace SafeCleaning;

	SafeRelease(pVertexBuffer_);
	SafeRelease(pIndexBuffer_);
}

//準備
HRESULT Sprite::Load(std::string fileName)
{
	//テクスチャ準備
	pTexture_ = new Texture();
	if(FAILED(pTexture_->Load(fileName)))
	{
		return E_FAIL;
	}

	//頂点情報準備
	InitVertex();

	//インデックス情報準備
	InitIndex();
	
	//コンスタントバッファ準備
	InitConstantBuffer();

	return S_OK;
}

//コンスタントバッファ準備
void Sprite::InitConstantBuffer()
{
	//必要な設定項目
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth =		sizeof(CONSTANT_BUFFER);
	cb.Usage =			D3D11_USAGE_DYNAMIC;
	cb.BindFlags =		D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags =		0;
	cb.StructureByteStride = 0;

	// 定数バッファの作成
	Direct3D::pDevice_->CreateBuffer(&cb, NULL, &pConstantBuffer_);
}


void Sprite::InitVertex()
{
	// 頂点データ宣言
	VERTEX vertices[] =
	{
		{ XMFLOAT3(-1.0f,  1.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f) },   // 四角形の頂点（左上）
		{ XMFLOAT3( 1.0f,  1.0f, 0.0f),	XMFLOAT3(1.0f, 0.0f, 0.0f) },   // 四角形の頂点（右上）
		{ XMFLOAT3(-1.0f, -1.0f, 0.0f),	XMFLOAT3(0.0f, 1.0f, 0.0f) },   // 四角形の頂点（左下）
		{ XMFLOAT3( 1.0f, -1.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 0.0f) },   // 四角形の頂点（右下）
	};


	// 頂点データ用バッファの設定
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(vertices);
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices;
	Direct3D::pDevice_->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);
}

void Sprite::InitIndex()
{
	int index[] = {2,1,0, 2,3,1 };

	// インデックスバッファを生成する
	D3D11_BUFFER_DESC   bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(index);
	bd.BindFlags = D3D10_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = index;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	Direct3D::pDevice_->CreateBuffer(&bd, &InitData, &pIndexBuffer_);
}



void Sprite::Draw(Transform& transform, RECT rect, float alpha)
{
	//いろいろ設定
	Direct3D::SetShader(Direct3D::SHADER_2D);

	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	Direct3D::pContext_->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);
	Direct3D::pContext_->VSSetConstantBuffers(0, 1, &pConstantBuffer_);
	Direct3D::pContext_->PSSetConstantBuffers(0, 1, &pConstantBuffer_);
	Direct3D::SetDepthBafferWriteEnable(false);


	// インデックスバッファーをセット
	stride = sizeof(int);
	offset = 0;
	Direct3D::pContext_->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);

	// パラメータの受け渡し
	D3D11_MAPPED_SUBRESOURCE pdata;
	CONSTANT_BUFFER cb;


	//表示するサイズに合わせる
	XMMATRIX cut = XMMatrixScaling((float)rect.right, (float)rect.bottom ,1);

	//画面に合わせる
	XMMATRIX view = XMMatrixScaling(1.0f / Direct3D::screenWidth_,
                                    1.0f / Direct3D::screenHeight_,
                                    1.0f);

	//最終的な行列
	XMMATRIX world = cut * transform.GetScaleMatrix() * transform.GetRotateMatrix() * view * transform.GetTranslateMatrix();
	cb.world = XMMatrixTranspose(world);

	// テクスチャ座標変換行列を渡す
	XMMATRIX mTexTrans = XMMatrixTranslation((float)rect.left / (float)pTexture_->GetSize().x,
		(float)rect.top / (float)pTexture_->GetSize().y, 0.0f);

	XMMATRIX mTexScale = XMMatrixScaling((float)rect.right / (float)pTexture_->GetSize().x,
		(float)rect.bottom / (float)pTexture_->GetSize().y, 1.0f);

	XMMATRIX mTexel = mTexScale * mTexTrans;
	cb.uvTrans = XMMatrixTranspose(mTexel);
	

	// テクスチャ合成色情報を渡す
	cb.color = XMFLOAT4(1, 1, 1, alpha);

	Direct3D::pContext_->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPUからのリソースアクセスを一時止める
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));		// リソースへ値を送る


	ID3D11SamplerState*			pSampler = pTexture_->GetSampler();
	Direct3D::pContext_->PSSetSamplers(0, 1, &pSampler);

	ID3D11ShaderResourceView*	pSRV = pTexture_->GetSRV();
	Direct3D::pContext_->PSSetShaderResources(0, 1, &pSRV);

	Direct3D::pContext_->Unmap(pConstantBuffer_, 0);									// GPUからのリソースアクセスを再開

	//ポリゴンメッシュを描画する
	Direct3D::pContext_->DrawIndexed(6, 0, 0);

	Direct3D::SetShader(Direct3D::SHADER_3D);

	Direct3D::SetDepthBafferWriteEnable(true);

}
