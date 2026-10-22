#include "SafeCleaning.hpp"
#include "Model.hpp"
#include "FbxParts.hpp"

using namespace SafeCleaning;

//3Dモデル（FBXファイル）を管理する
namespace Model
{
    //ロード済みのモデルデータ一覧
    std::vector<ModelData*>	_datas;

    ModelData* GetModelData(int _handle)
    {
        return _datas[_handle];
    }

    //初期化
    void Initialize()
    {
        AllRelease();
    }

    //モデルをロード
    int Load(std::string fileName)
    {
        ModelData* pData = new ModelData;

        //開いたファイル一覧から同じファイル名のものが無いか探す
        bool isExist = false;
        for (int i = 0; i < _datas.size(); i++)
        {
            //すでに開いている場合
            if (_datas[i] != nullptr && _datas[i]->fileName == fileName)
            {
                pData->pFbx = _datas[i]->pFbx;
                isExist = true;
                break;
            }
        }

        //新たにファイルを開く
        if (isExist == false)
        {
            pData->pFbx = new Fbx;
            if (FAILED(pData->pFbx->Load(fileName)))
            {
                //開けなかった
                SafeDelete(pData->pFbx);
                SafeDelete(pData);
                return -1;
            }

            //無事開けた
            pData->fileName = fileName;
        }

        pData->pFbx->CreateSkinInstance(pData->skin);

        //使ってない番号が無いか探す
        for (int i = 0; i < _datas.size(); i++)
        {
            if (_datas[i] == nullptr)
            {
                _datas[i] = pData;
                return i;
            }
        }

        //新たに追加
        _datas.push_back(pData);
        return (int)_datas.size() - 1;
    }

    void Update(int _handle)
    {
        if (_handle < 0 || _handle >= _datas.size() || _datas[_handle] == nullptr)
        {
            return;
        }

        auto& data{*_datas[_handle]};
        
        //アニメーションを進める
        data.nowFrame += data.animSpeed;

        //最後までアニメーションしたら戻す
        if (data.nowFrame > (float)data.endFrame)
        {
            data.nowFrame = (float)data.startFrame;
        }

        int frame = data.nowFrame;
        if (data.pFbx and frame != data.lastSkinFrame)
        {
            data.pFbx->UpdateSkin(frame, data.skin);
            data.lastSkinFrame = frame;
        }
    }

    //任意のモデルを開放
    void Release(int handle)
    {
        if (handle < 0 || handle >= _datas.size() || _datas[handle] == nullptr)
        {
            return;
        }

        //同じモデルを他でも使っていないか
        bool isExist = false;
        for (int i = 0; i < _datas.size(); i++)
        {
            //すでに開いている場合
            if (_datas[i] != nullptr && i != handle && _datas[i]->pFbx == _datas[handle]->pFbx)
            {
                isExist = true;
                break;
            }
        }

        //使ってなければモデル解放
        if (isExist == false)
        {
            SafeDelete(_datas[handle]->pFbx);
        }

        SafeDelete(_datas[handle]);
    }

    //全てのモデルを解放
    void AllRelease()
    {
        for (int i = 0; i < _datas.size(); i++)
        {
            if (_datas[i] != nullptr)
            {
                Release(i);
            }
        }
        _datas.clear();
    }

    //アニメーションのフレーム数をセット
    void SetAnimFrame(int handle, int startFrame, int endFrame, float animSpeed)
    {
        _datas[handle]->SetAnimFrame(startFrame, endFrame, animSpeed);
    }

    //現在のアニメーションのフレームを取得
    int GetAnimFrame(int handle)
    {
        return (int)_datas[handle]->nowFrame;
    }

    //任意のボーンの位置を取得
    XMFLOAT3 GetBonePosition(int handle, std::string boneName)
    {
        XMFLOAT3 pos = _datas[handle]->pFbx->GetBonePosition(boneName);
        XMVECTOR vec = XMVector3TransformCoord(XMLoadFloat3(&pos),
                                               _datas[handle]->transform.GetWorldMatrix());
        XMStoreFloat3(&pos, vec);
        return pos;
    }

    //ワールド行列を設定
    void SetTransform(int handle, Transform& transform)
    {
        if (handle < 0 || handle >= _datas.size())
        {
            return;
        }

        _datas[handle]->transform = transform;
    }

    //ワールド行列の取得
    XMMATRIX GetMatrix(int handle)
    {
        return _datas[handle]->transform.GetWorldMatrix();
    }

    ID3D11Buffer* GetVertexBuffer(int _handle, size_t _i)
    {
        ID3D11Buffer* pSkinVB{_datas[_handle]->skin.GetVertexBuffer(_i)};
        if (pSkinVB == nullptr)
        {
            return _datas[_handle]->pFbx->GetParts()[_i]->GetVertexBuffer();
        }
        
        return pSkinVB;
    }

    //レイキャスト（レイを飛ばして当たり判定）
    void RayCast(int handle, RayCastData* data)
    {
        XMFLOAT3 target = Transform::Float3Add(data->start, data->dir);
        XMMATRIX matInv = XMMatrixInverse(nullptr, _datas[handle]->transform.GetWorldMatrix());
        XMVECTOR vecStart = XMVector3TransformCoord(XMLoadFloat3(&data->start), matInv);
        XMVECTOR vecTarget = XMVector3TransformCoord(XMLoadFloat3(&target), matInv);
        XMVECTOR vecDir = vecTarget - vecStart;

        XMStoreFloat3(&data->start, vecStart);
        XMStoreFloat3(&data->dir, vecDir);

        _datas[handle]->pFbx->RayCast(data);
        // RayCast(_datas[handle]);
    }
}