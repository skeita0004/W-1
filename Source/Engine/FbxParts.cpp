#include "FbxParts.hpp"
#include "Fbx.hpp"
#include "SafeCleaning.hpp"
#include "Direct3D.hpp"
#include "CameraSet.hpp"

//コンストラクタ
FbxParts::FbxParts():
	ppIndexBuffer_(nullptr), pMaterial_(nullptr), 
	pVertexBuffer_(nullptr), pConstantBuffer_(nullptr),
	pVertexData_(nullptr), ppIndexData_(nullptr)
{
}

//デストラクタ
FbxParts::~FbxParts()
{
    using namespace SafeCleaning;
	SafeDeleteArray(pBoneArray_);
	SafeDeleteArray(ppCluster_);

	if (pWeightArray_ != nullptr)
	{
		for (DWORD i = 0; i < vertexCount_; i++)
		{
			SafeDeleteArray(pWeightArray_[i].pBoneIndex);
			SafeDeleteArray(pWeightArray_[i].pBoneWeight);
		}
		SafeDeleteArray(pWeightArray_);
	}



	SafeDeleteArray(pVertexData_);
	for (DWORD i = 0; i < materialCount_; i++)
	{
		SafeRelease(ppIndexBuffer_[i]);
		SafeDelete(ppIndexData_[i]);
		SafeDelete(pMaterial_[i].pTexture);

	}
	SafeDeleteArray(ppIndexBuffer_);
	SafeDeleteArray(ppIndexData_);
	SafeDeleteArray(pMaterial_);

	SafeRelease(pVertexBuffer_);
	SafeRelease(pConstantBuffer_);
}

//FBXファイルから情報をロードして諸々準備する
HRESULT FbxParts::Init(FbxNode *pNode)
{
	//ノードからメッシュの情報を取得
	FbxMesh* mesh = pNode->GetMesh();

	//各情報の個数を取得
	vertexCount_ = mesh->GetControlPointsCount();			//頂点の数
	polygonCount_ = mesh->GetPolygonCount();				//ポリゴンの数
	polygonVertexCount_ = mesh->GetPolygonVertexCount();	//ポリゴン頂点インデックス数 


	InitVertex(mesh);		//頂点バッファ準備
	InitMaterial(pNode);	//マテリアル準備
	InitIndex(mesh);		//インデックスバッファ準備
	InitSkelton(mesh);		//骨の情報を準備
	IntConstantBuffer();	//コンスタントバッファ（シェーダーに情報を送るやつ）準備

	return E_NOTIMPL;
}


//頂点バッファ準備
void FbxParts::InitVertex(fbxsdk::FbxMesh * mesh)
{
	pVertexData_ = new VERTEX[vertexCount_];

	for (DWORD poly = 0; poly < polygonCount_; poly++)
	{
		//3頂点分
		for (int vertex = 0; vertex < 3; vertex++)
		{
			int index = mesh->GetPolygonVertex(poly, vertex);

			/////////////////////////頂点の位置/////////////////////////////////////
			FbxVector4 pos = mesh->GetControlPointAt(index);
			pVertexData_[index].position = XMFLOAT3((float)pos[0], (float)pos[1], (float)pos[2]);

			/////////////////////////頂点の法線/////////////////////////////////////
			FbxVector4 Normal;
			mesh->GetPolygonVertexNormal(poly, vertex, Normal);	//ｉ番目のポリゴンの、ｊ番目の頂点の法線をゲット
			pVertexData_[index].normal = XMFLOAT3((float)Normal[0], (float)Normal[1], (float)Normal[2]);

			///////////////////////////頂点のＵＶ/////////////////////////////////////
			FbxLayerElementUV * pUV = mesh->GetLayer(0)->GetUVs();
			int uvIndex = mesh->GetTextureUVIndex(poly, vertex, FbxLayerElement::eTextureDiffuse);
			FbxVector2  uv = pUV->GetDirectArray().GetAt(uvIndex);
			pVertexData_[index].uv = XMFLOAT3((float)uv.mData[0], (float)(1.0f - uv.mData[1]), 0.0f);
		}
	}


	///////////////////////////頂点のＵＶ/////////////////////////////////////
	int m_dwNumUV = mesh->GetTextureUVCount();
	FbxLayerElementUV * pUV = mesh->GetLayer(0)->GetUVs();
	if (m_dwNumUV > 0 && pUV->GetMappingMode() == FbxLayerElement::eByControlPoint)
	{
		for (int k = 0; k < m_dwNumUV; k++)
		{
			FbxVector2 uv = pUV->GetDirectArray().GetAt(k);
			pVertexData_[k].uv = XMFLOAT3((float)uv.mData[0], (float)(1.0f - uv.mData[1]), 0.0f);
		}
	}


	// 頂点データ用バッファの設定
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(VERTEX) * mesh->GetControlPointsCount();
	bd_vertex.Usage = D3D11_USAGE_DYNAMIC;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = pVertexData_;
	Direct3D::pDevice_->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);


}

//マテリアル準備
void FbxParts::InitMaterial(fbxsdk::FbxNode * pNode)
{
	// マテリアルバッファの生成
	materialCount_ = pNode->GetMaterialCount();
	pMaterial_ = new MATERIAL[materialCount_];

	for (DWORD i = 0; i < materialCount_; i++)
	{
		ZeroMemory(&pMaterial_[i], sizeof(pMaterial_[i]));

		// フォンシェーディングを想定したマテリアルバッファの抽出
		FbxSurfaceMaterial* pMaterial = pNode->GetMaterial(i);
		FbxSurfacePhong* pPhong = (FbxSurfacePhong*)pMaterial;

		// 環境光＆拡散反射光＆鏡面反射光の反射成分値を取得
		FbxDouble3  ambient = FbxDouble3(0, 0, 0);
		FbxDouble3  diffuse = FbxDouble3(0, 0, 0);
		FbxDouble3  specular = FbxDouble3(0, 0, 0);
		ambient = pPhong->Ambient;
		diffuse = pPhong->Diffuse;

		// 環境光＆拡散反射光＆鏡面反射光の反射成分値をマテリアルバッファにコピー
		pMaterial_[i].ambient = XMFLOAT4((float)ambient[0], (float)ambient[1], (float)ambient[2], 1.0f);
		pMaterial_[i].diffuse = XMFLOAT4((float)diffuse[0], (float)diffuse[1], (float)diffuse[2], 1.0f);
		pMaterial_[i].specular = XMFLOAT4(0, 0, 0, 0);
		pMaterial_[i].shininess = 0;


		if (pMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
		{
			specular = pPhong->Specular;
			pMaterial_[i].specular = XMFLOAT4((float)specular[0], (float)specular[1], (float)specular[2], 1.0f);
			pMaterial_[i].shininess = (float)pPhong->Shininess;
		}


		InitTexture(pMaterial, i);

	}

}

//テクスチャ準備
void FbxParts::InitTexture(fbxsdk::FbxSurfaceMaterial * pMaterial, const DWORD &i)
{
	pMaterial_[i].pTexture = nullptr;


	// テクスチャー情報の取得
	FbxProperty  lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);

	//テクスチャの数
	int fileTextureCount = lProperty.GetSrcObjectCount<FbxFileTexture>();

	if (fileTextureCount > 0)
	{
		FbxFileTexture* texture = lProperty.GetSrcObject<FbxFileTexture>(0);

		//ファイル名+拡張だけにする
		char name[_MAX_FNAME];	//ファイル名
		char ext[_MAX_EXT];		//拡張子
        std::string txFileName{texture->GetRelativeFileName()};
		_splitpath_s(txFileName.c_str(), nullptr, 0, nullptr, 0, name, _MAX_FNAME, ext, _MAX_EXT);
		wsprintfA(name, "%s%s", name, ext);

		pMaterial_[i].pTexture = new Texture;

        std::string txName{name};
		pMaterial_[i].pTexture->Load(name);


	}
}

//インデックスバッファ準備
void FbxParts::InitIndex(fbxsdk::FbxMesh * mesh)
{
    using namespace SafeCleaning;

	// マテリアルの数だけインデックスバッファーを作成
	ppIndexBuffer_ = new ID3D11Buffer*[materialCount_];
	ppIndexData_ = new DWORD*[materialCount_];

	

	int count = 0;

	// マテリアルから「ポリゴン平面」の情報を抽出する
	for (DWORD i = 0; i < materialCount_; i++)
	{
		count = 0;
		DWORD *pIndex = new DWORD[polygonCount_ * 3];
		ZeroMemory(&pIndex[i], sizeof(pIndex[i]));

		// ポリゴンを構成する三角形平面が、
		// 「頂点バッファ」内のどの頂点を利用しているかを調べる
		for (DWORD j = 0; j < polygonCount_; j++)
		{
			FbxLayerElementMaterial *   mtl = mesh->GetLayer(0)->GetMaterials();
			int mtlId = mtl->GetIndexArray().GetAt(j);
			if (mtlId == i)
			{
				for (DWORD k = 0; k < 3; k++)
				{
					pIndex[count + k] = mesh->GetPolygonVertex(j, k);
				}
				count += 3;
			}
		}

		// インデックスバッファを生成する
		D3D11_BUFFER_DESC   bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(int) * count;
		bd.BindFlags = D3D10_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = pIndex;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;
		if (FAILED(Direct3D::pDevice_->CreateBuffer(&bd, &InitData, &ppIndexBuffer_[i])))
		{
			//MessageBox(0, "インデックスバッファの生成に失敗", fbxFileName, MB_OK);
			//return FALSE;
		}
		pMaterial_[i].polygonCount = count / 3;
		ppIndexData_[i] = new DWORD[count];
		memcpy(ppIndexData_[i], pIndex, sizeof(DWORD) * count);
		SafeDeleteArray(pIndex);
	}

}

//骨の情報を準備
void FbxParts::InitSkelton(FbxMesh * pMesh)
{
    using namespace SafeCleaning;

	// デフォーマ情報（ボーンとモデルの関連付け）の取得
	FbxDeformer *   pDeformer = pMesh->GetDeformer(0);
	if (pDeformer == nullptr)
	{
		//ボーン情報なし
		return;
	}


	// デフォーマ情報からスキンメッシュ情報を取得
	pSkinInfo_ = (FbxSkin *)pDeformer;

	// 頂点からポリゴンを逆引きするための情報を作成する
	struct  POLY_INDEX
	{
		int *   polyIndex;      // ポリゴンの番号
		int *   vertexIndex;    // 頂点の番号
		int     numRef;         // 頂点を共有するポリゴンの数
	};

	POLY_INDEX * polyTable = new POLY_INDEX[vertexCount_];
	for (DWORD i = 0; i < vertexCount_; i++)
	{
		// 三角形ポリゴンに合わせて、頂点とポリゴンの関連情報を構築する
		// 総頂点数＝ポリゴン数×３頂点
		polyTable[i].polyIndex = new int[polygonCount_ * 3];
		polyTable[i].vertexIndex = new int[polygonCount_ * 3];
		polyTable[i].numRef = 0;
		ZeroMemory(polyTable[i].polyIndex, sizeof(int)* polygonCount_ * 3);
		ZeroMemory(polyTable[i].vertexIndex, sizeof(int)* polygonCount_ * 3);

		// ポリゴン間で共有する頂点を列挙する
		for (DWORD k = 0; k < polygonCount_; k++)
		{
			for (int m = 0; m < 3; m++)
			{
				if (pMesh->GetPolygonVertex(k, m) == i)
				{
					polyTable[i].polyIndex[polyTable[i].numRef] = k;
					polyTable[i].vertexIndex[polyTable[i].numRef] = m;
					polyTable[i].numRef++;
				}
			}
		}
	}

	// ボーン情報を取得する
	numBone_ = pSkinInfo_->GetClusterCount();
	ppCluster_ = new FbxCluster*[numBone_];
	for (int i = 0; i < numBone_; i++)
	{
		ppCluster_[i] = pSkinInfo_->GetCluster(i);
	}

	// ボーンの数に合わせてウェイト情報を準備する
	pWeightArray_ = new FbxParts::Weight[vertexCount_];
	for (DWORD i = 0; i < vertexCount_; i++)
	{
		pWeightArray_[i].posOrigin = pVertexData_[i].position;
		pWeightArray_[i].normalOrigin = pVertexData_[i].normal;
		pWeightArray_[i].pBoneIndex = new int[numBone_];
		pWeightArray_[i].pBoneWeight = new float[numBone_];
		for (int j = 0; j < numBone_; j++)
		{
			pWeightArray_[i].pBoneIndex[j] = -1;
			pWeightArray_[i].pBoneWeight[j] = 0.0f;
		}
	}

	// それぞれのボーンに影響を受ける頂点を調べる
	// そこから逆に、頂点ベースでボーンインデックス・重みを整頓する
	for (int i = 0; i < numBone_; i++)
	{
		int numIndex = ppCluster_[i]->GetControlPointIndicesCount();   //このボーンに影響を受ける頂点数
		int * piIndex = ppCluster_[i]->GetControlPointIndices();       //ボーン/ウェイト情報の番号
		double * pdWeight = ppCluster_[i]->GetControlPointWeights();     //頂点ごとのウェイト情報

																				 //頂点側からインデックスをたどって、頂点サイドで整理する
		for (int j = 0; j < numIndex; j++)
		{
			// 頂点に関連付けられたウェイト情報がボーン５本以上の場合は、重みの大きい順に４本に絞る
			for (int k = 0; k < 4; k++)
			{
				if (k >= numBone_)
					break;

				if (pdWeight[j] > pWeightArray_[piIndex[j]].pBoneWeight[k])
				{
					for (int m = numBone_ - 1; m > k; m--)
					{
						pWeightArray_[piIndex[j]].pBoneIndex[m]  = pWeightArray_[piIndex[j]].pBoneIndex[m - 1];
						pWeightArray_[piIndex[j]].pBoneWeight[m] = pWeightArray_[piIndex[j]].pBoneWeight[m - 1];
					}
					pWeightArray_[piIndex[j]].pBoneIndex[k]  = i;
					pWeightArray_[piIndex[j]].pBoneWeight[k] = (float)pdWeight[j];
					break;
				}
			}
		}
	}

	//ボーンを生成
	pBoneArray_ = new FbxParts::Bone[numBone_];
	for (int i = 0; i < numBone_; i++)
	{
		// ボーンのデフォルト位置を取得する
		FbxAMatrix  matrix;
		ppCluster_[i]->GetTransformLinkMatrix(matrix);

		// 行列コピー（Fbx形式からDirectXへの変換）
		XMFLOAT4X4 pose;
		for (DWORD x = 0; x < 4; x++)
		{
			for (DWORD y = 0; y < 4; y++)
			{
				pose(x,y) = (float)matrix.Get(x, y);
			}
		}
		pBoneArray_[i].bindPose = XMLoadFloat4x4(&pose);
	}

	// 一時的なメモリ領域を解放する
	for (DWORD i = 0; i < vertexCount_; i++)
	{
		SafeDeleteArray(polyTable[i].polyIndex);
		SafeDeleteArray(polyTable[i].vertexIndex);
	}
	SafeDeleteArray(polyTable);

}

//コンスタントバッファ（シェーダーに情報を送るやつ）準備
void FbxParts::IntConstantBuffer()
{
	// 定数バッファの作成(パラメータ受け渡し用)
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	Direct3D::pDevice_->CreateBuffer(&cb, NULL, &pConstantBuffer_);
}

//描画
void FbxParts::Draw(Transform& transform)
{
    static CameraSet cameraSet{};
	//今から描画する頂点情報をシェーダに伝える
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	Direct3D::pContext_->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

	//使用するコンスタントバッファをシェーダに伝える
	Direct3D::pContext_->VSSetConstantBuffers(0, 1, &pConstantBuffer_);
	Direct3D::pContext_->PSSetConstantBuffers(0, 1, &pConstantBuffer_);


	//シェーダーのコンスタントバッファーに各種データを渡す
	for (DWORD i = 0; i < materialCount_; i++)
	{
		// インデックスバッファーをセット
		UINT    stride = sizeof(int);
		UINT    offset = 0;
		Direct3D::pContext_->IASetIndexBuffer(ppIndexBuffer_[i], DXGI_FORMAT_R32_UINT, 0);


		// パラメータの受け渡し
		D3D11_MAPPED_SUBRESOURCE pdata;
		CONSTANT_BUFFER cb;
		cb.worldVewProj =	XMMatrixTranspose(transform.GetWorldMatrix() * cameraSet.GetCurrent()->GetViewMat() * cameraSet.GetCurrent()->GetProjectionMat());						// リソースへ送る値をセット
		cb.world =		XMMatrixTranspose(transform.GetWorldMatrix());
		cb.normalTrans =	XMMatrixTranspose(transform.GetRotateMatrix() * XMMatrixInverse(nullptr, transform.GetScaleMatrix()));
		cb.ambient = pMaterial_[i].ambient;
		cb.diffuse = pMaterial_[i].diffuse;
		cb.speculer = pMaterial_[i].specular;
		cb.shininess = pMaterial_[i].shininess;
		cb.cameraPosition = XMFLOAT4(cameraSet.GetCurrent()->GetTransform().position.x,
                                     cameraSet.GetCurrent()->GetTransform().position.y,
                                     cameraSet.GetCurrent()->GetTransform().position.z,
                                     0);
		cb.lightDirection = XMFLOAT4(0, -1, 0, 0);
		cb.isTexture = pMaterial_[i].pTexture != nullptr;


		Direct3D::pContext_->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPUからのリソースアクセスを一時止める
		memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));		// リソースへ値を送る



		// テクスチャをシェーダーに設定

		if (cb.isTexture)
		{
			ID3D11SamplerState*			pSampler = pMaterial_[i].pTexture->GetSampler();
			Direct3D::pContext_->PSSetSamplers(0, 1, &pSampler);

			ID3D11ShaderResourceView*	pSRV = pMaterial_[i].pTexture->GetSRV();
			Direct3D::pContext_->PSSetShaderResources(0, 1, &pSRV);
		}
		Direct3D::pContext_->Unmap(pConstantBuffer_, 0);									// GPUからのリソースアクセスを再開

		 //ポリゴンメッシュを描画する
		Direct3D::pContext_->DrawIndexed(pMaterial_[i].polygonCount * 3, 0, 0);
	}

}

//ボーン有りのモデルを描画
void FbxParts::DrawSkinAnime(Transform& transform, FbxTime time)
{
	// ボーンごとの現在の行列を取得する
	for (int i = 0; i < numBone_; i++)
	{
		FbxAnimEvaluator * evaluator = ppCluster_[i]->GetLink()->GetScene()->GetAnimationEvaluator();
		FbxMatrix mCurrentOrentation = evaluator->GetNodeGlobalTransform(ppCluster_[i]->GetLink(), time);

		// 行列コピー（Fbx形式からDirectXへの変換）
		XMFLOAT4X4 pose;
		for (DWORD x = 0; x < 4; x++)
		{
			for (DWORD y = 0; y < 4; y++)
			{
				pose(x, y) = (float)mCurrentOrentation.Get(x, y);
			}
		}

		// オフセット時のポーズの差分を計算する
		pBoneArray_[i].newPose = XMLoadFloat4x4(&pose);
		pBoneArray_[i].diffPose = XMMatrixInverse(nullptr, pBoneArray_[i].bindPose);
		pBoneArray_[i].diffPose *= pBoneArray_[i].newPose;
	}

	// 各ボーンに対応した頂点の変形制御
	for (DWORD i = 0; i < vertexCount_; i++)
	{
		// 各頂点ごとに、「影響するボーン×ウェイト値」を反映させた関節行列を作成する
		XMMATRIX  matrix;
		ZeroMemory(&matrix, sizeof(matrix));
		for (int m = 0; m < numBone_; m++)
		{
			if (pWeightArray_[i].pBoneIndex[m] < 0)
			{
				break;
			}
			matrix += pBoneArray_[pWeightArray_[i].pBoneIndex[m]].diffPose * pWeightArray_[i].pBoneWeight[m];

		}

		// 作成された関節行列を使って、頂点を変形する
		XMVECTOR Pos = XMLoadFloat3(&pWeightArray_[i].posOrigin);
		XMVECTOR Normal = XMLoadFloat3(&pWeightArray_[i].normalOrigin);
		XMStoreFloat3(&pVertexData_[i].position,XMVector3TransformCoord(Pos, matrix));
		XMStoreFloat3(&pVertexData_[i].normal, XMVector3TransformCoord(Normal, matrix));

	}

	// 頂点バッファをロックして、変形させた後の頂点情報で上書きする
	D3D11_MAPPED_SUBRESOURCE msr = {};
	Direct3D::pContext_->Map(pVertexBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	if (msr.pData)
	{
		memcpy_s(msr.pData, msr.RowPitch, pVertexData_, sizeof(VERTEX) * vertexCount_);
		Direct3D::pContext_->Unmap(pVertexBuffer_, 0);
	}


	Draw(transform);

}

void FbxParts::DrawMeshAnime(Transform& transform, FbxTime time, FbxScene * scene)
{
	//// その瞬間の自分の姿勢行列を得る
	//FbxAnimEvaluator *evaluator = scene->GetAnimationEvaluator();
	//FbxMatrix mCurrentOrentation = evaluator->GetNodeGlobalTransform(_pNode, time);

	//// Fbx形式の行列からDirectX形式の行列へのコピー（4×4の行列）
	//for (DWORD x = 0; x < 4; x++)
	//{
	//	for (DWORD y = 0; y < 4; y++)
	//	{
	//		_localMatrix(x, y) = (float)mCurrentOrentation.Get(x, y);
	//	}
	//}

	Draw(transform);
}

bool FbxParts::GetBonePosition(std::string boneName, XMFLOAT3 * position)
{
	for (int i = 0; i < numBone_; i++)
	{
		if (boneName == ppCluster_[i]->GetLink()->GetName())
		{
			FbxAMatrix  matrix;
			ppCluster_[i]->GetTransformLinkMatrix(matrix);

			position->x = (float)matrix[3][0];
			position->y = (float)matrix[3][1];
			position->z = (float)matrix[3][2];

			return true;
		}
	}
	return false;
}

void FbxParts::RayCast(RayCastData * data)
{
	data->hit = FALSE;

	//マテリアル毎
	for (DWORD i = 0; i < materialCount_; i++)
	{
		//そのマテリアルのポリゴン毎
		for (DWORD j = 0; j < pMaterial_[i].polygonCount; j++)
		{
			//3頂点
			XMFLOAT3 ver[3];
			ver[0] = pVertexData_[ppIndexData_[i][j * 3 + 0]].position;
			ver[1] = pVertexData_[ppIndexData_[i][j * 3 + 1]].position;
			ver[2] = pVertexData_[ppIndexData_[i][j * 3 + 2]].position;

			BOOL  hit = FALSE;
			float dist = 0.0f;

			hit = Direct3D::Intersect(data->start, data->dir, ver[0], ver[1], ver[2], &dist);


			if (hit && dist < data->dist)
			{
				data->hit = TRUE;
				data->dist = dist;
			}
		}
	}
}
