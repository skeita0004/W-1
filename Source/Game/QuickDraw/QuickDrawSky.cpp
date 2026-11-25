#include "QuickDrawSky.hpp"

#include "Direct3D.hpp"
#include "SafeCleaning.hpp"

#include <cassert>
#include <cstring>

QuickDrawSky::QuickDrawSky(GameObject* parent)
    : GameObject(parent, "QuickDrawSky")
{
}

QuickDrawSky::~QuickDrawSky()
{
    Release();
}

void QuickDrawSky::Initialize()
{
    CreateVertexBuffer();
    CreateConstantBuffer();
    CreateTexture();
    CreateSampler();
    CreateDepthState();
}

void QuickDrawSky::Update()
{
}

void QuickDrawSky::Draw()
{
    if (!Direct3D::pContext_ ||
        !vertexBuffer_ ||
        !constantBuffer_ ||
        !skySRV_ ||
        !sampler_)
    {
        return;
    }

    auto* context = Direct3D::pContext_;

    //Skyシェーダー
    Direct3D::SetShader(Direct3D::SHADER_SKY);

    //頂点バッファ
    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    context->IASetVertexBuffers(
        0,
        1,
        &vertexBuffer_,
        &stride,
        &offset
    );

    context->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
    );

    //VS定数バッファ
    context->VSSetConstantBuffers(
        0,
        1,
        &constantBuffer_
    );

    //PS定数バッファ
    context->PSSetConstantBuffers(
        0,
        1,
        &constantBuffer_
    );

    //テクスチャ
    context->PSSetShaderResources(
        0,
        1,
        &skySRV_
    );

    //サンプラー
    context->PSSetSamplers(
        0,
        1,
        &sampler_
    );

    //深度無効化
    context->OMSetDepthStencilState(
        depthState_,
        0
    );

    //背景描画
    context->Draw(6, 0);

    //テクスチャ解除
    ID3D11ShaderResourceView* nullSRV = nullptr;

    context->PSSetShaderResources(
        0,
        1,
        &nullSRV
    );

    //深度状態を戻す
    context->OMSetDepthStencilState(
        nullptr,
        0
    );
}

void QuickDrawSky::Release()
{
    SafeCleaning::SafeRelease(vertexBuffer_);
    SafeCleaning::SafeRelease(constantBuffer_);

    SafeCleaning::SafeRelease(skyTexture_);
    SafeCleaning::SafeRelease(skySRV_);
    SafeCleaning::SafeRelease(sampler_);

    SafeCleaning::SafeRelease(depthState_);
}

void QuickDrawSky::CreateVertexBuffer()
{
    Vertex vertices[] =
    {
        //左上
        {
            XMFLOAT3(-1.0f,  1.0f, 0.0f),
            XMFLOAT3(0.0f, -1.0f, 0.0f),
            XMFLOAT2(0.0f, 0.0f)
        },

        //右上
        {
            XMFLOAT3(1.0f,  1.0f, 0.0f),
            XMFLOAT3(0.0f, -1.0f, 0.0f),
            XMFLOAT2(1.0f, 0.0f)
        },

        //左下
        {
            XMFLOAT3(-1.0f, -1.0f, 0.0f),
            XMFLOAT3(0.0f, -1.0f, 0.0f),
            XMFLOAT2(0.0f, 1.0f)
        },

        //右上
        {
            XMFLOAT3(1.0f,  1.0f, 0.0f),
            XMFLOAT3(0.0f, -1.0f, 0.0f),
            XMFLOAT2(1.0f, 0.0f)
        },

        //右下
        {
            XMFLOAT3(1.0f, -1.0f, 0.0f),
            XMFLOAT3(0.0f, -1.0f, 0.0f),
            XMFLOAT2(1.0f, 1.0f)
        },

        //左下
        {
            XMFLOAT3(-1.0f, -1.0f, 0.0f),
            XMFLOAT3(0.0f, -1.0f, 0.0f),
            XMFLOAT2(0.0f, 1.0f)
        }
    };

    D3D11_BUFFER_DESC desc{};
    desc.ByteWidth = sizeof(vertices);
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA data{};
    data.pSysMem = vertices;

    HRESULT hr = Direct3D::pDevice_->CreateBuffer(
        &desc,
        &data,
        &vertexBuffer_
    );

    assert(SUCCEEDED(hr));
}

void QuickDrawSky::CreateConstantBuffer()
{
    SkyConstantBuffer cb{};

    //WVP
    XMStoreFloat4x4(
        &cb.matWVP,
        XMMatrixIdentity()
    );

    //法線変換
    XMStoreFloat4x4(
        &cb.matNormal,
        XMMatrixIdentity()
    );

    //ワールド変換
    XMStoreFloat4x4(
        &cb.matWorld,
        XMMatrixIdentity()
    );

    //ライト方向
    cb.lightDir = XMFLOAT4(
        0.0f,
        1.0f,
        0.0f,
        0.0f
    );

    //ディフューズ
    cb.diffuse = XMFLOAT4(
        1.0f,
        1.0f,
        1.0f,
        1.0f
    );

    //アンビエント
    cb.ambient = XMFLOAT4(
        0.0f,
        0.0f,
        0.0f,
        0.0f
    );

    //スペキュラー
    cb.specular = XMFLOAT4(
        0.0f,
        0.0f,
        0.0f,
        0.0f
    );

    //カメラ位置
    cb.cameraPosition = XMFLOAT4(
        0.0f,
        0.0f,
        1.0f,
        1.0f
    );

    //光沢
    cb.shininess = 1.0f;

    //テクスチャ使用
    cb.isTexture = TRUE;

    D3D11_BUFFER_DESC desc{};
    desc.ByteWidth = sizeof(SkyConstantBuffer);
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    D3D11_SUBRESOURCE_DATA data{};
    data.pSysMem = &cb;

    HRESULT hr = Direct3D::pDevice_->CreateBuffer(
        &desc,
        &data,
        &constantBuffer_
    );

    assert(SUCCEEDED(hr));
}

void QuickDrawSky::CreateTexture()
{
    //上部：明るい空色
    //下部：少し濃い空色

    const unsigned int pixels[] =
    {
        0xFFFFE4B9,
        0xFFFFE4B9,

        0xFFF0B872,
        0xFFF0B872
    };

    D3D11_TEXTURE2D_DESC desc{};

    desc.Width = 2;
    desc.Height = 2;
    desc.MipLevels = 1;
    desc.ArraySize = 1;

    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    desc.SampleDesc.Count = 1;

    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SUBRESOURCE_DATA data{};

    data.pSysMem = pixels;
    data.SysMemPitch = sizeof(unsigned int) * 2;
    data.SysMemSlicePitch = sizeof(pixels);

    HRESULT hr = Direct3D::pDevice_->CreateTexture2D(
        &desc,
        &data,
        &skyTexture_
    );

    assert(SUCCEEDED(hr));

    hr = Direct3D::pDevice_->CreateShaderResourceView(
        skyTexture_,
        nullptr,
        &skySRV_
    );

    assert(SUCCEEDED(hr));
}

void QuickDrawSky::CreateSampler()
{
    D3D11_SAMPLER_DESC desc{};

    desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

    desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

    desc.MaxAnisotropy = 1;

    desc.MinLOD = 0.0f;
    desc.MaxLOD = D3D11_FLOAT32_MAX;

    HRESULT hr = Direct3D::pDevice_->CreateSamplerState(
        &desc,
        &sampler_
    );

    assert(SUCCEEDED(hr));
}

void QuickDrawSky::CreateDepthState()
{
    D3D11_DEPTH_STENCIL_DESC desc{};

    desc.DepthEnable = FALSE;

    desc.DepthWriteMask =
        D3D11_DEPTH_WRITE_MASK_ZERO;

    desc.DepthFunc =
        D3D11_COMPARISON_ALWAYS;

    desc.StencilEnable = FALSE;

    HRESULT hr = Direct3D::pDevice_->CreateDepthStencilState(
        &desc,
        &depthState_
    );

    assert(SUCCEEDED(hr));
}