#include "BillBoard.hpp"
#include "CameraSet.hpp"
#include "SafeCleaning.hpp"

BillBoard::BillBoard():
	pVertexBuffer_(nullptr),
    pIndexBuffer_(nullptr),
    pConstantBuffer_(nullptr),
    pTexture_(nullptr),
    pCameraSet_(nullptr)
{
}

BillBoard::~BillBoard()
{
}

//画像をロードしてポリゴンを作成
HRESULT BillBoard::Load(std::string fileName)
{
	HRESULT hr;

    pCameraSet_ = new CameraSet();

	// 頂点情報
	VERTEX vertices[] =
	{
		{ XMFLOAT3(-0.5f, 0.5f, 0.0f),XMFLOAT3(0.0f, 0.0f, 0.0f) },   // 四角形の頂点（左上）
		{ XMFLOAT3(0.5f, 0.5f, 0.0f),	XMFLOAT3(1.0f, 0.0f, 0.0f) },   // 四角形の頂点（右上）
		{ XMFLOAT3(0.5f, -0.5f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 0.0f) },   // 四角形の頂点（右下）
		{ XMFLOAT3(-0.5f, -0.5f, 0.0f),XMFLOAT3(0.0f, 1.0f, 0.0f) },   // 四角形の頂点（左下）
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
	hr = Direct3D::pDevice_->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);
	if (FAILED(hr))
	{
		return hr;
	}



	//インデックス情報
	int index[] = { 0,2,3, 0,1,2 };

	// インデックスバッファを生成する
	D3D11_BUFFER_DESC   bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(index);
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = index;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	hr = Direct3D::pDevice_->CreateBuffer(&bd, &InitData, &pIndexBuffer_);
	if (FAILED(hr))
	{
		return hr;
	}

	//コンスタントバッファ作成
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	// コンスタントバッファの作成
	Direct3D::pDevice_->CreateBuffer(&cb, nullptr, &pConstantBuffer_);


	//テクスチャ
	pTexture_ = new Texture;
	pTexture_->Load(fileName.c_str());

	return S_OK;
}


//描画
void BillBoard::Draw(XMMATRIX matWorld, XMFLOAT4 color)
{
	//コンスタントバッファに渡す情報
	CONSTANT_BUFFER cb;
	cb.matWVP = XMMatrixTranspose(matWorld * pCameraSet_->GetCurrent()->GetViewMat() * pCameraSet_->GetCurrent()->GetProjectionMat());
	cb.color = color;

	D3D11_MAPPED_SUBRESOURCE pdata;
	Direct3D::pContext_->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPUからのデータアクセスを止める
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));	// データを値を送る

	ID3D11SamplerState* pSampler = pTexture_->GetSampler();
	Direct3D::pContext_->PSSetSamplers(0, 1, &pSampler);

	ID3D11ShaderResourceView* pSRV = pTexture_->GetSRV();
	Direct3D::pContext_->PSSetShaderResources(0, 1, &pSRV);

	Direct3D::pContext_->Unmap(pConstantBuffer_, 0);	//再開



	//頂点バッファ
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	Direct3D::pContext_->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

	// インデックスバッファーをセット
	stride = sizeof(int);
	offset = 0;
	Direct3D::pContext_->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);

	//コンスタントバッファ
	Direct3D::pContext_->VSSetConstantBuffers(0, 1, &pConstantBuffer_);	//頂点シェーダー用	
	Direct3D::pContext_->PSSetConstantBuffers(0, 1, &pConstantBuffer_);	//ピクセルシェーダー用

	Direct3D::pContext_->DrawIndexed(6, 0, 0);
}


//解放
void BillBoard::Release()
{
    using namespace SafeCleaning;
	SafeDelete(pTexture_);
	SafeRelease(pConstantBuffer_);
    SafeRelease(pIndexBuffer_);
	SafeRelease(pVertexBuffer_);
}