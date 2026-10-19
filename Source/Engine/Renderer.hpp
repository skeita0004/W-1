#pragma once

#include <windows.h>

#include <span>

#include "Direct3D.hpp"
#include "Texture.hpp"

#include "Model.hpp"

#include "FbxParts.hpp"

class  Transform;
struct Material;

class Renderer
{
public:
    Renderer();
    ~Renderer();

    void Init();

    void Draw(int _modelHandle);
    void DrawShadowMap(int _modelHandle);
    void DrawReflectionMap(int _modelHandle);

    void Release();

    void SendLightPos(XMFLOAT3 _lightPos)
    {
        lightPos_ = XMFLOAT4(_lightPos.x,
                             _lightPos.y,
                             _lightPos.z,
                             1.0f);
    }
    void SendLightTarget(XMFLOAT4 _lightTar)
    {
        lightTar_ = XMFLOAT4(_lightTar.x,
                             _lightTar.y,
                             _lightTar.z,
                             1.0f);
    }

private:
    struct alignas(16) MatrixBuffer
    {
        XMMATRIX	matWVP;
        XMMATRIX	matNormal;
        XMMATRIX    matW;
        XMMATRIX    matP;
        XMMATRIX    matVP;
        XMMATRIX    matInvW;
        XMMATRIX    matRefView;
    };

    struct alignas(16) ShadowMatrixBuffer
    {
        XMMATRIX   matWVP;
    };

    struct alignas(16) MaterialBuffer
    {
        XMFLOAT4    diffuseColor;
        XMFLOAT4    ambientColor;
        XMFLOAT4    specularColor;
        XMFLOAT4    emissiveColor;

        //XMFLOAT4    params;
        float       transparency;
        float       diffuseFactor;
        float       specularPower;

        int         isTexture;
        int         isNormalMap;
    };

    struct alignas(16) RefMaterialBuffer
    {
        XMFLOAT4 diffuseColor;
        XMINT4 isTexture;
    };

    struct alignas(16) LightBuffer
    {
        XMFLOAT4 lightPos;
        XMFLOAT4 lightDir;

        float    lightAttenuation;
        float    conePhi;

        XMFLOAT2    padding;
    };

    struct alignas(16) CameraBuffer
    {
        XMFLOAT4    cameraPos;
    };

    Texture* pNormalMap_;

    inline static XMFLOAT4 lightPos_{
        0.f,
        2.f,
        0.f,
        1.f
    };

    inline static XMFLOAT4 lightTar_{
        0.f,
        0.f,
        0.f,
        0.f
    };

    // コンスタントバッファ
    ID3D11Buffer* pMatrixBuffer_;
    ID3D11Buffer* pShadowMatrixBuffer_;
    ID3D11Buffer* pMaterialBuffer_;
    ID3D11Buffer* pRefMaterialBuffer_;
    ID3D11Buffer* pLightBuffer_;
    ID3D11Buffer* pCameraBuffer_;

    HRESULT InitConstantBuffer();

    void Draw(Transform&                   _transform,
              int32_t                      _materialCount,
              ID3D11Buffer*                _pVertexBuffer,
              std::span<ID3D11Buffer*>     _pIndexBuffer,
              std::span<int32_t>           _indexCountArr,
              const std::vector<Material>& _materialList);

    void DrawShadowMap(Transform& _transform,
                       int32_t                           _materialCount,
                       ID3D11Buffer* _pVertexBuffer,
                       std::span<ID3D11Buffer*>          _pIndexBuffer,
                       std::span<int32_t>                _indexCountArr,
                       const std::vector<Material>& _materialList);

    void DrawReflectionMap(Transform& _transform,
                           int32_t                           _materialCount,
                           ID3D11Buffer* _pVertexBuffer,
                           std::span<ID3D11Buffer*>          _pIndexBuffer,
                           std::span<int32_t>                _indexCountArr,
                           const std::vector<Material>& _materialList);

    //void DrawReflect(Transform& _transform,
    //                 int32_t                           _materialCount,
    //                 ID3D11Buffer* _pVertexBuffer,
    //                 std::span<ID3D11Buffer*>          _pIndexBuffer,
    //                 std::span<int32_t>                _indexCountArr,
    //                 const std::vector<Material>& _materialList);
};