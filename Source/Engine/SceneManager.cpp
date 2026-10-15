#include "sceneManager.hpp"

#include "Model.hpp"
#include "Image.hpp"
#include "Audio.hpp"


//コンストラクタ
SceneManager::SceneManager(GameObject * parent)
	: GameObject(parent, "SceneManager"),
    currentSceneID_(INITIAL_SCENE),
    nextSceneID_(INITIAL_SCENE)
{
}

//初期化
void SceneManager::Init()
{
	//最初のシーンを準備
	currentSceneID_ = INITIAL_SCENE; // ここもiniから取れるようになればいいのに。
	nextSceneID_ = currentSceneID_;
    InstantiateScene(currentSceneID_, this);
}

//更新
void SceneManager::Update()
{
	//次のシーンが現在のシーンと違う　＝　シーンを切り替えなければならない
	if (currentSceneID_ != nextSceneID_)
	{
		//そのシーンのオブジェクトを全削除
		DeleteAllChildren();

		//ロードしたデータを全削除
		Audio::Release();
		Model::AllRelease();
		Image::AllRelease();

        //次のシーンを作成
        InstantiateScene(nextSceneID_, this);

		Audio::Initialize();
		currentSceneID_ = nextSceneID_;
	} // endif
}

//描画
void SceneManager::Draw()
{
}

//開放
void SceneManager::Release()
{
}

//シーン切り替え（実際に切り替わるのはこの次のフレーム）
void SceneManager::ChangeScene(SceneID next)
{
	nextSceneID_ = next;
}

GameObject* SceneManager::InstantiateScene(SceneID _sid, GameObject* _pParent)
{
    size_t index = static_cast<size_t>(_sid);
    return (this->*sceneTable[index])(_pParent);
}
