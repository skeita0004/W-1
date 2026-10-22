#pragma once

#include <windows.h>

#include <span>

#include "Direct3D.hpp"
#include "Texture.hpp"

#include "Model.hpp"

#include "FbxParts.hpp"

class  Transform;
struct Material;

namespace Renderer
{
    void Init();

    // いらなくね？
    void Release();

    void Draw(int _modelHandle);
    void DrawShadowMap(int _modelHandle);
    void DrawReflectionMap(int _modelHandle);

    void Release();

    inline static XMFLOAT4 lightPos_{
        0.f,
        0.f,
        0.f,
        1.f
    };

    inline static XMFLOAT4 lightTar_{
        0.f,
        1.f,
        -1.f,
        0.f
    };

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

    extern Texture* pNormalMap_;

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