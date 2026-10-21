#include "Fbx.hpp"
#include "Direct3D.hpp"
#include "FbxParts.hpp"
#include "SkinInstance.hpp"

Fbx::Fbx():_animSpeed(0)
{
}


Fbx::~Fbx()
{
	for (int i = 0; i < parts_.size(); i++)
	{
		delete parts_[i];
	}
	parts_.clear();

	pFbxScene_->Destroy();
	pFbxManager_->Destroy();
}

HRESULT Fbx::Load(const std::string& fileName)
{
	// FBXの読み込み
	pFbxManager_ = FbxManager::Create();
	pFbxScene_ = FbxScene::Create(pFbxManager_, "fbxscene");
	FbxString FileName(fileName.c_str());
	FbxImporter *fbxImporter = FbxImporter::Create(pFbxManager_, "imp");
	if (!fbxImporter->Initialize(FileName.Buffer(), -1, pFbxManager_->GetIOSettings()))
	{
		//失敗
		return E_FAIL;
	}
	fbxImporter->Import(pFbxScene_);
	fbxImporter->Destroy();

	// アニメーションのタイムモードの取得
	_frameRate = pFbxScene_->GetGlobalSettings().GetTimeMode();


	//現在のカレントディレクトリを覚えておく
	wchar_t defaultCurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

	//カレントディレクトリをファイルがあった場所に変更
	wchar_t dir[MAX_PATH];
    std::wstring wFileName{fileName.begin(), fileName.end()};
	_wsplitpath_s(wFileName.c_str(), nullptr, 0, dir, MAX_PATH, nullptr, 0, nullptr, 0);
	SetCurrentDirectory(dir);



	//ルートノードを取得して
	FbxNode* rootNode = pFbxScene_->GetRootNode();

	//そいつの子供の数を調べて
	int childCount = rootNode->GetChildCount();

	//1個ずつチェック
	for (int i = 0; childCount > i; i++)
	{
		CheckNode(rootNode->GetChild(i), &parts_);
	}

	//カレントディレクトリを元の位置に戻す
	SetCurrentDirectory(defaultCurrentDir);

	return S_OK;
}

void Fbx::CheckNode(FbxNode * pNode, std::vector<FbxParts*>* pPartsList)
{
	//そのノードにはメッシュ情報が入っているだろうか？
	FbxNodeAttribute* attr = pNode->GetNodeAttribute();
	if (attr != nullptr && attr->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		//パーツを用意
		FbxParts* pParts = new FbxParts;
		pParts->Init(pNode);

		//パーツ情報を動的配列に追加
		pPartsList->push_back(pParts);
	}


	//子ノードにもデータがあるかも！！
	{
		//子供の数を調べて
		int childCount = pNode->GetChildCount();

		//一人ずつチェック
		for (int i = 0; i < childCount; i++)
		{
			CheckNode(pNode->GetChild(i), pPartsList);
		}
	}
}


void Fbx::CreateSkinInstance(SkinInstance& _inst) const
{
	_inst.Parts().resize(parts_.size());
	for (size_t i = 0; i < parts_.size(); i++)
	{
		if (parts_[i]->GetSkinInfo() != nullptr)
		{
			parts_[i]->CreateSkinInstance(_inst.Parts()[i]);
		}
	}
}

void Fbx::UpdateSkin(int _frame, SkinInstance& _inst) const
{
	FbxTime     time;
	time.SetTime(0, 0, 0, _frame, 0, 0, _frameRate);

	for (size_t i = 0; i < parts_.size(); i++)
	{
		//スキンアニメーション（ボーン有り）の場合
		if (parts_[i]->GetSkinInfo() != nullptr)
		{
			parts_[i]->Skinning(time, _inst.Parts()[i]);
		}
	}
}

void Fbx::Release()
{

}

XMFLOAT3 Fbx::GetBonePosition(std::string boneName)
{
	XMFLOAT3 position = XMFLOAT3(0, 0, 0);
	for (int i = 0; i < parts_.size(); i++)
	{
		if (parts_[i]->GetBonePosition(boneName, &position))
			break;
	}


	return position;
}


//レイキャスト（レイを飛ばして当たり判定）
void Fbx::RayCast(RayCastData * data)
{
	//すべてのパーツと判定
	for (int i = 0; i < parts_.size(); i++)
	{
		parts_[i]->RayCast(data);
	}
}
