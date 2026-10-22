#include "Renderer.hpp"

#include "Transform.hpp"
#include "CameraSet.hpp"

#include <string>
#include <filesystem>

namespace Renderer
{
    // コンスタントバッファ
    ID3D11Buffer* pMatrixBuffer_;
    ID3D11Buffer* pShadowMatrixBuffer_;
    ID3D11Buffer* pMaterialBuffer_;
    ID3D11Buffer* pRefMaterialBuffer_;
    ID3D11Buffer* pLightBuffer_;
    ID3D11Buffer* pCameraBuffer_;

    Texture* pNormalMap_;

    void Init()
    {
        HRESULT result = InitConstantBuffer();
        if (FAILED(result))
        {
        }
    }

    void Draw(int _modelHandle)
    {
        Direct3D::SetBlendMode(Direct3D::BLEND_DEFAULT);
        Model::ModelData* modelData{Model::GetModelData(_modelHandle)};
        auto              parts{modelData->pFbx->GetParts()};

        for (size_t i = 0; i < parts.size(); i++)
        {
            Draw(modelData->transform,
                 parts[i]->GetMaterialCount(),
                 Model::GetVertexBuffer(_modelHandle, i),
                 parts[i]->GetIndexBuffer(),
                 parts[i]->GetIndexCountArray(),
                 parts[i]->GetMaterialList());
        }
    }

    void DrawShadowMap(int _modelHandle)
    {
        Model::ModelData* modelData{Model::GetModelData(_modelHandle)};
        auto              parts{modelData->pFbx->GetParts()};

        for (size_t i = 0; i < parts.size(); i++)
        {
            DrawShadowMap(modelData->transform,
                          parts[i]->GetMaterialCount(),
                          Model::GetVertexBuffer(_modelHandle, i),
                          parts[i]->GetIndexBuffer(),
                          parts[i]->GetIndexCountArray(),
                          parts[i]->GetMaterialList());
        }
    }

    void DrawReflectionMap(int _modelHandle)
    {
        Model::ModelData* modelData{Model::GetModelData(_modelHandle)};
        auto              parts{modelData->pFbx->GetParts()};

        for (size_t i = 0; i < parts.size(); i++)
        {
            DrawReflectionMap(modelData->transform,
                              parts[i]->GetMaterialCount(),
                              Model::GetVertexBuffer(_modelHandle, i),
                              parts[i]->GetIndexBuffer(),
                              parts[i]->GetIndexCountArray(),
                              parts[i]->GetMaterialList());
        }
    }

    void Draw(Transform& _transform,
                        int32_t                           _materialCount,
                        ID3D11Buffer* _pVertexBuffer,
                        std::span<ID3D11Buffer*>          _pIndexBuffer,
                        std::span<int32_t>                _indexCountArr,
                        const std::vector<Material>& _materialList)
    {
        Direct3D::SetShader(Direct3D::SHADER_3D);

        //頂点バッファ
        UINT stride = sizeof(Vertex);
        UINT offset = 0;
        Direct3D::pContext_->IASetVertexBuffers(0, 1, &_pVertexBuffer, &stride, &offset);

        //コンスタントバッファ
        Direct3D::pContext_->VSSetConstantBuffers(0, 1, &pMatrixBuffer_);
        Direct3D::pContext_->VSSetConstantBuffers(2, 1, &pLightBuffer_);

        Direct3D::pContext_->PSSetConstantBuffers(1, 1, &pMaterialBuffer_);
        Direct3D::pContext_->PSSetConstantBuffers(2, 1, &pLightBuffer_);
        Direct3D::pContext_->PSSetConstantBuffers(3, 1, &pCameraBuffer_);


        for (int i = 0; i < _materialCount; i++) // materialCountのゲッタを用意すること。
        {
            CameraSet cameraSet{};

            // インデックスバッファー
            stride = sizeof(UINT);
            offset = 0;
            Direct3D::pContext_->IASetIndexBuffer(_pIndexBuffer[i], DXGI_FORMAT_R32_UINT, 0);

            XMMATRIX worldMat = _transform.GetWorldMatrix(); // ここで計算されるのですわ～

            // 行列用コンスタントバッファ
            D3D11_MAPPED_SUBRESOURCE pdata{};

            size_t bufferSize = sizeof(MatrixBuffer);

            const auto& normalCamera = cameraSet.GetVirtualCamera(CameraSet::hNormalCamera);
            const auto& shadowCamera = cameraSet.GetVirtualCamera(CameraSet::hShadowCamera);
            const auto& reflectionCamera = cameraSet.GetVirtualCamera(CameraSet::hReflectionCamera);

            MatrixBuffer mCb{};

            mCb.matWVP = worldMat * normalCamera->GetViewMat() * normalCamera->GetProjectionMat();
            mCb.matP = normalCamera->GetProjectionMat();
            mCb.matVP = shadowCamera->GetViewMat() * shadowCamera->GetProjectionMat();
            mCb.matNormal = _transform.GetNormalMatrix();
            mCb.matW = worldMat;
            mCb.matInvW = XMMatrixInverse(nullptr, worldMat);
            mCb.matRefView = reflectionCamera->GetViewMat();

            Direct3D::pContext_->Map(pMatrixBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPUからのデータアクセスを止める
            memcpy_s(pdata.pData, bufferSize, (void*)(&mCb), bufferSize);	// データの値を送る
            Direct3D::pContext_->Unmap(pMatrixBuffer_, 0);	//再開


            // マテリアル用コンスタントバッファ

            bufferSize = sizeof(MaterialBuffer);

            MaterialBuffer materialCb{};
            materialCb.diffuseColor = _materialList[i].diffuse;
            materialCb.ambientColor = _materialList[i].ambient;
            materialCb.specularColor = _materialList[i].specular;
            materialCb.emissiveColor = _materialList[i].emissive;
            materialCb.transparency = _materialList[i].transparency;
            materialCb.diffuseFactor = _materialList[i].diffuseFactor;
            materialCb.specularPower = _materialList[i].shininess;
            materialCb.isTexture = (INT)(_materialList[i].pTexture != nullptr);
            materialCb.isNormalMap = (INT)(pNormalMap_ != nullptr);

            Direct3D::pContext_->Map(pMaterialBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);
            memcpy_s(pdata.pData, bufferSize, (void*)(&materialCb), bufferSize);
            Direct3D::pContext_->Unmap(pMaterialBuffer_, 0);


            // ライトの向きを求める
            // LightSetクラスを作ったほうがいい
            XMVECTOR vLightPos{XMLoadFloat4(&lightPos_)};
            XMVECTOR vLightTar{XMLoadFloat4(&lightTar_)};

            XMVECTOR vLightDir{XMVector3Normalize((vLightTar - vLightPos))};

            XMFLOAT4 lightDir{};
            XMStoreFloat4(&lightDir, vLightDir);

            // ライト用コンスタントバッファ
            {
                D3D11_MAPPED_SUBRESOURCE pdata;
                size_t bufferSize = sizeof(LightBuffer);

                LightBuffer lightCb{};
                lightCb.lightPos = lightPos_;
                lightCb.lightDir = lightDir;
                lightCb.lightAttenuation = 0.3f;
                lightCb.conePhi = std::cosf(XMConvertToRadians(30.f));

                Direct3D::pContext_->Map(pLightBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);
                memcpy_s(pdata.pData, bufferSize, (void*)(&lightCb), bufferSize);
                Direct3D::pContext_->Unmap(pLightBuffer_, 0);
            }

            // カメラ用コンスタントバッファ
            {
                D3D11_MAPPED_SUBRESOURCE pdata{};
                size_t bufferSize = sizeof(CameraBuffer);

                CameraBuffer camCb{};

                auto& cameraPosRaw = normalCamera->GetTransform().position;
                camCb.cameraPos = {
                    cameraPosRaw.x,
                    cameraPosRaw.y,
                    cameraPosRaw.z,
                    1.0f
                };

                Direct3D::pContext_->Map(pCameraBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);
                memcpy_s(pdata.pData, bufferSize, (void*)(&camCb), bufferSize);
                Direct3D::pContext_->Unmap(pCameraBuffer_, 0);
            }

            // テクスチャ類をGPUに送信↓

            // モデルのテクスチャ
            if (_materialList[i].pTexture != nullptr)
            {
                ID3D11SamplerState* pSampler = _materialList[i].pTexture->GetSampler();
                ID3D11ShaderResourceView* pSRV = _materialList[i].pTexture->GetSRV();

                Direct3D::pContext_->PSSetSamplers(0, 1, &pSampler);
                Direct3D::pContext_->PSSetShaderResources(0, 1, &pSRV);
            }
            else
            {
                ID3D11SamplerState* pNullSampler = nullptr;
                ID3D11ShaderResourceView* pNullSRV = nullptr;

                Direct3D::pContext_->PSSetSamplers(0, 1, &pNullSampler);
                Direct3D::pContext_->PSSetShaderResources(0, 1, &pNullSRV);
            }

            // モデルのノーマルマップ
            if (pNormalMap_ != nullptr)
            {
                ID3D11SamplerState* pNormalMapSampler = pNormalMap_->GetSampler();
                ID3D11ShaderResourceView* pNormalMapSRV = pNormalMap_->GetSRV();

                Direct3D::pContext_->PSSetSamplers(2, 1, &pNormalMapSampler);
                Direct3D::pContext_->PSSetShaderResources(2, 1, &pNormalMapSRV);
            }

            // シャドウマップ
            {
                ID3D11SamplerState* pShadowSampler = Direct3D::pShadowSamplerState;
                ID3D11ShaderResourceView* pShadowSRV = Direct3D::pShadowSRV;

                Direct3D::pContext_->PSSetSamplers(1, 1, &pShadowSampler);
                Direct3D::pContext_->PSSetShaderResources(1, 1, &pShadowSRV);
            }

            // 映り込みマップ
            {
                ID3D11SamplerState* pRefSampler = Direct3D::pReflectionSamplerState;
                ID3D11ShaderResourceView* pRefSRV = Direct3D::pReflectionMapSRV;

                Direct3D::pContext_->PSSetSamplers(3, 1, &pRefSampler);
                Direct3D::pContext_->PSSetShaderResources(3, 1, &pRefSRV);
            }

            Direct3D::pContext_->DrawIndexed(_indexCountArr[i], 0, 0);
        }
    }

    void DrawShadowMap(Transform& _transform,
                                 int32_t                           _materialCount,
                                 ID3D11Buffer* _pVertexBuffer,
                                 std::span<ID3D11Buffer*>          _pIndexBuffer,
                                 std::span<int32_t>                _indexCountArr,
                                 const std::vector<Material>& _materialList)
    {
        Direct3D::SetShader(Direct3D::SHADER_SHADOW_MAP);

        CameraSet cameraSet{};

        //頂点バッファ
        UINT stride = sizeof(Vertex);
        UINT offset = 0;
        Direct3D::pContext_->IASetVertexBuffers(0, 1, &_pVertexBuffer, &stride, &offset);

        //コンスタントバッファ
        Direct3D::pContext_->VSSetConstantBuffers(0, 1, &pShadowMatrixBuffer_);

        for (int i = 0; i < _materialCount; i++)
        {
            // インデックスバッファー
            stride = sizeof(UINT);
            offset = 0;
            Direct3D::pContext_->IASetIndexBuffer(_pIndexBuffer[i], DXGI_FORMAT_R32_UINT, 0);

            XMMATRIX worldMat = _transform.GetWorldMatrix(); // ここで計算されるのですわ～
            D3D11_MAPPED_SUBRESOURCE pdata{};

            size_t bufferSize = sizeof(ShadowMatrixBuffer);

            // 行列用コンスタントバッファ
            ShadowMatrixBuffer mCb{};
            pdata.RowPitch = sizeof(ShadowMatrixBuffer);


            const auto& shadowCamera = cameraSet.GetVirtualCamera(CameraSet::hShadowCamera);

            // ワールド変換行列に、ライトからのビューと射影変換行列をかける
            mCb.matWVP = worldMat * shadowCamera->GetViewMat() * shadowCamera->GetProjectionMat();
            //mCb.matWVP = XMMatrixIdentity();

            Direct3D::pContext_->Map(pShadowMatrixBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPUからのデータアクセスを止める
            memcpy_s(pdata.pData, bufferSize, &mCb, bufferSize);	// データの値を送る
            Direct3D::pContext_->Unmap(pShadowMatrixBuffer_, 0);	//再開

            Direct3D::pContext_->DrawIndexed(_indexCountArr[i], 0, 0);
        }
    }

    void DrawReflectionMap(Transform& _transform,
                                     int32_t                           _materialCount,
                                     ID3D11Buffer* _pVertexBuffer,
                                     std::span<ID3D11Buffer*>          _pIndexBuffer,
                                     std::span<int32_t>                _indexCountArr,
                                     const std::vector<Material>& _materialList)
    {
        Direct3D::SetShader(Direct3D::SHADER_REFLECTION_MAP);

        CameraSet cameraSet{};

        //頂点バッファ
        UINT stride = sizeof(Vertex);
        UINT offset = 0;
        Direct3D::pContext_->IASetVertexBuffers(0, 1, &_pVertexBuffer, &stride, &offset);

        //コンスタントバッファ
        Direct3D::pContext_->VSSetConstantBuffers(0, 1, &pShadowMatrixBuffer_);
        Direct3D::pContext_->PSSetConstantBuffers(1, 1, &pRefMaterialBuffer_);

        for (int i = 0; i < _materialCount; i++) // materialCountのゲッタを用意すること。
        {
            // インデックスバッファー
            stride = sizeof(UINT);
            offset = 0;
            Direct3D::pContext_->IASetIndexBuffer(_pIndexBuffer[i], DXGI_FORMAT_R32_UINT, 0);

            XMMATRIX worldMat = _transform.GetWorldMatrix(); // ここで計算されるのですわ～ <- 誰？ <- 何？

            // 行列用コンスタントバッファ
            {
                D3D11_MAPPED_SUBRESOURCE pdata;
                size_t bufferSize = sizeof(ShadowMatrixBuffer);

                const auto& normalCamera = cameraSet.GetVirtualCamera(CameraSet::hNormalCamera);
                const auto& reflectionCamera = cameraSet.GetVirtualCamera(CameraSet::hReflectionCamera);

                ShadowMatrixBuffer mCb{};
                mCb.matWVP = worldMat * reflectionCamera->GetViewMat() * normalCamera->GetProjectionMat();

                Direct3D::pContext_->Map(pShadowMatrixBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPUからのデータアクセスを止める
                memcpy_s(pdata.pData, bufferSize, (void*)(&mCb), bufferSize);	// データの値を送る
                Direct3D::pContext_->Unmap(pShadowMatrixBuffer_, 0);	//再開
            }

            // マテリアル用コンスタントバッファ
            {
                D3D11_MAPPED_SUBRESOURCE pdata;
                size_t bufferSize = sizeof(RefMaterialBuffer);

                RefMaterialBuffer materialCb{};
                materialCb.diffuseColor = _materialList[i].diffuse;
                materialCb.isTexture = XMINT4((INT)(_materialList[i].pTexture != nullptr),
                                              0, 0, 0);

                Direct3D::pContext_->Map(pRefMaterialBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);
                memcpy_s(pdata.pData, bufferSize, (void*)(&materialCb), bufferSize);
                Direct3D::pContext_->Unmap(pRefMaterialBuffer_, 0);
            }

            // テクスチャ類をGPUに送信↓
            // モデルのテクスチャ
            if (_materialList[i].pTexture != nullptr)
            {
                ID3D11SamplerState* pSampler = _materialList[i].pTexture->GetSampler();
                ID3D11ShaderResourceView* pSRV = _materialList[i].pTexture->GetSRV();

                Direct3D::pContext_->PSSetSamplers(0, 1, &pSampler);
                Direct3D::pContext_->PSSetShaderResources(0, 1, &pSRV);
            }
            else
            {
                ID3D11SamplerState* pNullSampler = nullptr;
                ID3D11ShaderResourceView* pNullSRV = nullptr;

                Direct3D::pContext_->PSSetSamplers(0, 1, &pNullSampler);
                Direct3D::pContext_->PSSetShaderResources(0, 1, &pNullSRV);
            }

            Direct3D::pContext_->DrawIndexed(_indexCountArr[i], 0, 0);
        }
    }

    //void Renderer::DrawReflect(Transform &_transform,
    //                           int32_t _materialCount,
    //                           ID3D11Buffer *_pVertexBuffer,
    //                           std::span<ID3D11Buffer *> _pIndexBuffer,

    //                           std::span<int32_t> _indexCountArr,
    //                           const std::vector<Material> &_materialList)
    //{
    //    Direct3D::SetShader(Direct3D::SHADER_REFLECT);
    //
    //    //頂点バッファ
    //    UINT stride = sizeof(VERTEX);
    //    UINT offset = 0;
    //    Direct3D::pContext_->IASetVertexBuffers(0, 1, &_pVertexBuffer, &stride, &offset);
    //
    //    //コンスタントバッファ
    //    Direct3D::pContext_->VSSetConstantBuffers(0, 1, &pMatrixBuffer_);
    //    Direct3D::pContext_->VSSetConstantBuffers(2, 1, &pLightBuffer_);
    //
    //    Direct3D::pContext_->PSSetConstantBuffers(1, 1, &pMaterialBuffer_);
    //    Direct3D::pContext_->PSSetConstantBuffers(2, 1, &pLightBuffer_);
    //    Direct3D::pContext_->PSSetConstantBuffers(3, 1, &pCameraBuffer_);
    //
    //
    //    for (int i = 0; i < _materialCount; i++) // materialCountのゲッタを用意すること。
    //    {
    //        // インデックスバッファー
    //        stride = sizeof(UINT);
    //        offset = 0;
    //        Direct3D::pContext_->IASetIndexBuffer(_pIndexBuffer[i], DXGI_FORMAT_R32_UINT, 0);
    //
    //        XMMATRIX worldMat = _transform.GetWorldMatrix(); // ここで計算されるのですわ～
    //
    //        // 行列用コンスタントバッファ
    //        D3D11_MAPPED_SUBRESOURCE pdata{};
    //
    //        size_t bufferSize = sizeof(MatrixBuffer);
    //
    //        MatrixBuffer mCb{};
    //        mCb.matWVP = worldMat * Camera::GetViewMatrix() * Camera::GetProjectionMatrix();
    //        mCb.matVP = ShadowCamera::GetViewMatrix() * ShadowCamera::GetProjectionMatrix();
    //        mCb.matNormal = _transform.GetNormalMatrix();
    //        mCb.matW = worldMat;
    //        mCb.matInvW = XMMatrixInverse(nullptr, worldMat);
    //        mCb.matRefView = ReflectionCamera::GetViewMatrix();
    //        mCb.matP = Camera::GetProjectionMatrix();
    //
    //        Direct3D::pContext_->Map(pMatrixBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPUからのデータアクセスを止める
    //        memcpy_s(pdata.pData, bufferSize, (void *)(&mCb), bufferSize);	// データの値を送る
    //        Direct3D::pContext_->Unmap(pMatrixBuffer_, 0);	//再開
    //
    //
    //        // マテリアル用コンスタントバッファ
    //
    //        bufferSize = sizeof(MaterialBuffer);
    //
    //        MaterialBuffer materialCb{};
    //        materialCb.diffuseColor = _materialList[i].diffuse;
    //        materialCb.ambientColor = _materialList[i].ambient;
    //        materialCb.specularColor = _materialList[i].specular;
    //        materialCb.emissiveColor = _materialList[i].emissive;
    //        materialCb.diffuseFactor = _materialList[i].diffuseFactor;
    //        materialCb.specularPower = _materialList[i].shininess;
    //        materialCb.isTexture = (INT)(_materialList[i].pTexture != nullptr);
    //        materialCb.isNormalMap = (INT)(pNormalMap_ != nullptr);
    //
    //        Direct3D::pContext_->Map(pMaterialBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);
    //        memcpy_s(pdata.pData, bufferSize, (void *)(&materialCb), bufferSize);
    //        Direct3D::pContext_->Unmap(pMaterialBuffer_, 0);
    //
    //
    //        // ライトの向きを求める
    //        // LightSetクラスを作ったほうがいい
    //        XMVECTOR vLightPos{ XMLoadFloat4(&lightPos_) };
    //        XMVECTOR vLightTar{ XMLoadFloat4(&lightTar_) };
    //
    //        XMVECTOR vLightDir{ XMVector3Normalize((vLightTar - vLightPos)) };
    //
    //        XMFLOAT4 lightDir{};
    //        XMStoreFloat4(&lightDir, vLightDir);
    //
    //        // ライト用コンスタントバッファ
    //        {
    //            D3D11_MAPPED_SUBRESOURCE pdata;
    //            size_t bufferSize = sizeof(LightBuffer);
    //
    //            LightBuffer lightCb{};
    //            lightCb.lightPos = lightPos_;
    //            lightCb.lightDir = lightDir;
    //            lightCb.lightAttenuation = 0.3f;
    //            lightCb.conePhi = std::cosf(XMConvertToRadians(30.f));
    //
    //            Direct3D::pContext_->Map(pLightBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);
    //            memcpy_s(pdata.pData, bufferSize, (void *)(&lightCb), bufferSize);
    //            Direct3D::pContext_->Unmap(pLightBuffer_, 0);
    //        }
    //
    //        // カメラ用コンスタントバッファ
    //        {
    //            D3D11_MAPPED_SUBRESOURCE pdata{};
    //            size_t bufferSize = sizeof(CameraBuffer);
    //
    //            CameraBuffer camCb{};
    //            camCb.cameraPos = Camera::GetPosition();
    //
    //            Direct3D::pContext_->Map(pCameraBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);
    //            memcpy_s(pdata.pData, bufferSize, (void *)(&camCb), bufferSize);
    //            Direct3D::pContext_->Unmap(pCameraBuffer_, 0);
    //        }
    //
    //        // テクスチャ類をGPUに送信↓
    //
    //        // モデルのテクスチャ
    //        if (_materialList[i].pTexture != nullptr)
    //        {
    //            ID3D11SamplerState *pSampler = _materialList[i].pTexture->GetSampler();
    //            ID3D11ShaderResourceView *pSRV = _materialList[i].pTexture->GetSRV();
    //
    //            Direct3D::pContext_->PSSetSamplers(0, 1, &pSampler);
    //            Direct3D::pContext_->PSSetShaderResources(0, 1, &pSRV);
    //        }
    //        else
    //        {
    //            ID3D11SamplerState *pNullSampler = nullptr;
    //            ID3D11ShaderResourceView *pNullSRV = nullptr;
    //
    //            Direct3D::pContext_->PSSetSamplers(0, 1, &pNullSampler);
    //            Direct3D::pContext_->PSSetShaderResources(0, 1, &pNullSRV);
    //        }
    //
    //        // モデルのノーマルマップ
    //        if (pNormalMap_ != nullptr)
    //        {
    //            ID3D11SamplerState *pNormalMapSampler = pNormalMap_->GetSampler();
    //            ID3D11ShaderResourceView *pNormalMapSRV = pNormalMap_->GetSRV();
    //
    //            Direct3D::pContext_->PSSetSamplers(2, 1, &pNormalMapSampler);
    //            Direct3D::pContext_->PSSetShaderResources(2, 1, &pNormalMapSRV);
    //        }
    //
    //        // シャドウマップ
    //        {
    //            ID3D11SamplerState *pShadowSampler = Direct3D::pShadowSamplerState;
    //            ID3D11ShaderResourceView *pShadowSRV = Direct3D::pShadowSRV;
    //
    //            Direct3D::pContext_->PSSetSamplers(1, 1, &pShadowSampler);
    //            Direct3D::pContext_->PSSetShaderResources(1, 1, &pShadowSRV);
    //        }
    //
    //        // 映り込みマップ
    //        {
    //            ID3D11SamplerState *pRefSampler = Direct3D::pReflectionSamplerState;
    //            ID3D11ShaderResourceView *pRefSRV = Direct3D::pReflectionMapSRV;
    //
    //            Direct3D::pContext_->PSSetSamplers(3, 1, &pRefSampler);
    //            Direct3D::pContext_->PSSetShaderResources(3, 1, &pRefSRV);
    //        }
    //
    //        Direct3D::pContext_->DrawIndexed(_indexCountArr[i], 0, 0);
    //    }
    //}

    void Release()
    {
    }

    HRESULT InitConstantBuffer()
    {
        // コンスタントバッファ類の定義

        // 通常描画パス用座標変換行列群
        D3D11_BUFFER_DESC matCbDesc{};
        matCbDesc.ByteWidth = sizeof(Renderer::MatrixBuffer);
        matCbDesc.Usage = D3D11_USAGE_DYNAMIC;
        matCbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        matCbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        matCbDesc.MiscFlags = 0;
        matCbDesc.StructureByteStride = 0;

        // WVP変換行列のみ
        D3D11_BUFFER_DESC shadowMatCbDesc{};
        shadowMatCbDesc.ByteWidth = sizeof(Renderer::ShadowMatrixBuffer);
        shadowMatCbDesc.Usage = D3D11_USAGE_DYNAMIC;
        shadowMatCbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        shadowMatCbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        shadowMatCbDesc.MiscFlags = 0;
        shadowMatCbDesc.StructureByteStride = 0;

        // 通常描画パス用マテリアル群
        D3D11_BUFFER_DESC materialCbDesc{};
        materialCbDesc.ByteWidth = sizeof(Renderer::MaterialBuffer);
        materialCbDesc.Usage = D3D11_USAGE_DYNAMIC;
        materialCbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        materialCbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        materialCbDesc.MiscFlags = 0;
        materialCbDesc.StructureByteStride = 0;

        // 映り込みマップ用マテリアル群
        D3D11_BUFFER_DESC refMaterialCbDesc{};
        refMaterialCbDesc.ByteWidth = sizeof(Renderer::RefMaterialBuffer);
        refMaterialCbDesc.Usage = D3D11_USAGE_DYNAMIC;
        refMaterialCbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        refMaterialCbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        refMaterialCbDesc.MiscFlags = 0;
        refMaterialCbDesc.StructureByteStride = 0;

        // 光源関係
        D3D11_BUFFER_DESC lightCbDesc{};
        lightCbDesc.ByteWidth = sizeof(Renderer::LightBuffer);
        lightCbDesc.Usage = D3D11_USAGE_DYNAMIC;
        lightCbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        lightCbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        lightCbDesc.MiscFlags = 0;
        lightCbDesc.StructureByteStride = 0;

        // カメラ関係
        D3D11_BUFFER_DESC cameraCbDesc{};
        cameraCbDesc.ByteWidth = sizeof(Renderer::CameraBuffer);
        cameraCbDesc.Usage = D3D11_USAGE_DYNAMIC;
        cameraCbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cameraCbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        cameraCbDesc.MiscFlags = 0;
        cameraCbDesc.StructureByteStride = 0;

        HRESULT result{};
        result = Direct3D::pDevice_->CreateBuffer(&matCbDesc, nullptr, &pMatrixBuffer_);
        result = Direct3D::pDevice_->CreateBuffer(&shadowMatCbDesc, nullptr, &pShadowMatrixBuffer_);
        result = Direct3D::pDevice_->CreateBuffer(&materialCbDesc, nullptr, &pMaterialBuffer_);
        result = Direct3D::pDevice_->CreateBuffer(&refMaterialCbDesc, nullptr, &pRefMaterialBuffer_);
        result = Direct3D::pDevice_->CreateBuffer(&lightCbDesc, nullptr, &pLightBuffer_);
        result = Direct3D::pDevice_->CreateBuffer(&cameraCbDesc, nullptr, &pCameraBuffer_);

        if (FAILED(result))
        {
            std::wstring errorMsg{std::filesystem::path{std::system_category().message(result)}.wstring()};
            MessageBox(nullptr, errorMsg.c_str(), L"FAILED", MB_OK);
            return result;
        }

        return result;
    }
}