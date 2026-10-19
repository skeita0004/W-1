#pragma once
#include <DirectXMath.h>

#include "Texture.hpp"

struct Material
{
    DWORD             polygonCount;  //マテリアルのポリゴン数

    DirectX::XMFLOAT4 diffuse;       // 拡散反射色
    float             diffuseFactor; // 拡散反射強度
    float             transparency;  // 透明度

    DirectX::XMFLOAT4 ambient;       //環境光（アンビエント）への反射強度

    DirectX::XMFLOAT4 specular;      // 鏡面反射色
    float             shininess;     // 鏡面反射強度

    DirectX::XMFLOAT4 emissive;      // 放射

    Texture*          pTexture;      //テクスチャ
};