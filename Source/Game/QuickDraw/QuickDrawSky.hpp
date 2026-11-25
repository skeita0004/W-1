#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#include "GameObject.hpp"

using namespace DirectX;

class QuickDrawSky : public GameObject
{
private:
    struct Vertex
    {
        XMFLOAT3 position;
        XMFLOAT3 normal;
        XMFLOAT2 uv;
    };

    struct alignas(16) SkyConstantBuffer
    {
        XMFLOAT4X4 matWVP;
        XMFLOAT4X4 matNormal;
        XMFLOAT4X4 matWorld;

        XMFLOAT4 lightDir;
        XMFLOAT4 diffuse;
        XMFLOAT4 ambient;
        XMFLOAT4 specular;
        XMFLOAT4 cameraPosition;

        float shininess;
        BOOL isTexture;

        float padding[2];
    };

    ID3D11Buffer* vertexBuffer_ = nullptr;
    ID3D11Buffer* constantBuffer_ = nullptr;

    ID3D11Texture2D* skyTexture_ = nullptr;
    ID3D11ShaderResourceView* skySRV_ = nullptr;
    ID3D11SamplerState* sampler_ = nullptr;

    ID3D11DepthStencilState* depthState_ = nullptr;

public:
    QuickDrawSky(GameObject* parent);
    ~QuickDrawSky() override;

    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

private:
    void CreateVertexBuffer();
    void CreateConstantBuffer();
    void CreateTexture();
    void CreateSampler();
    void CreateDepthState();
};