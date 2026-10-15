#include "Texture.hpp"
#include "Direct3D.hpp"
#include "SafeCleaning.hpp"

Texture::Texture():
	pSampleLinear_(nullptr), pTextureSRV_(nullptr), size_(XMFLOAT3(0,0,0))
{
}

Texture::~Texture()
{
    using namespace SafeCleaning;

	SafeRelease(pSampleLinear_);
	SafeRelease(pTextureSRV_);
}

HRESULT Texture::Load(std::string fileName)
{
	wchar_t wtext[FILENAME_MAX];
	size_t ret;
	mbstowcs_s(&ret, wtext, fileName.c_str(), fileName.length());


	// テクスチャを読み込む
	CoInitialize(NULL);
	IWICImagingFactory *pFactory = NULL;
	IWICBitmapDecoder *pDecoder = NULL;
	IWICBitmapFrameDecode* pFrame = NULL;
	IWICFormatConverter* pFormatConverter = NULL;
	CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, reinterpret_cast<void **>(&pFactory));
	HRESULT hr = pFactory->CreateDecoderFromFilename(wtext, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &pDecoder);
	if(FAILED(hr))
	{
		wchar_t message[256];
		wsprintf(message, L"「%s」が見つかりません", fileName.c_str());
		MessageBox(0, message, L"画像ファイルの読み込みに失敗", MB_OK);
		return hr;
	}
	pDecoder->GetFrame(0, &pFrame);
	pFactory->CreateFormatConverter(&pFormatConverter);
	pFormatConverter->Initialize(pFrame, GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone, NULL, 1.0f, WICBitmapPaletteTypeMedianCut);
	UINT imgWidth;
	UINT imgHeight;
	pFormatConverter->GetSize(&imgWidth, &imgHeight);
	size_ = XMFLOAT3((float)imgWidth, (float)imgHeight, 0);

	// テクスチャの設定
	ID3D11Texture2D*	pTexture;			// テクスチャデータ
	D3D11_TEXTURE2D_DESC texdec;
	texdec.Width = imgWidth;
	texdec.Height = imgHeight;
	texdec.MipLevels = 1;
	texdec.ArraySize = 1;
	texdec.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texdec.SampleDesc.Count = 1;
	texdec.SampleDesc.Quality = 0;
	texdec.Usage = D3D11_USAGE_DYNAMIC;
	texdec.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texdec.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	texdec.MiscFlags = 0;
	Direct3D::pDevice_->CreateTexture2D(&texdec, NULL, &pTexture);

	// テクスチャを送る
	D3D11_MAPPED_SUBRESOURCE hMappedres;
	Direct3D::pContext_->Map(pTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &hMappedres);
	pFormatConverter->CopyPixels(NULL, imgWidth * 4, imgWidth * imgHeight * 4, (BYTE*)hMappedres.pData);
	Direct3D::pContext_->Unmap(pTexture, 0);


	// シェーダリソースビュー(テクスチャ用)の設定
	D3D11_SHADER_RESOURCE_VIEW_DESC srv = {};
	srv.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv.Texture2D.MipLevels = 1;
	Direct3D::pDevice_->CreateShaderResourceView(pTexture, &srv, &pTextureSRV_);


	// テクスチャー用サンプラー作成
	D3D11_SAMPLER_DESC  SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	Direct3D::pDevice_->CreateSamplerState(&SamDesc, &pSampleLinear_);

	pTexture->Release();
	pFormatConverter->Release();
	pFrame->Release();
	pDecoder->Release();
	pFactory->Release();

	return S_OK;
}
