#pragma once
#include "GameObject.hpp"

#include "../Game/TestScene.hpp"
#include "../Game/TitleScene.hpp"
#include "../Game/PlayScene.hpp"
#include "../Game/ClearScene.hpp"
#include "../Game/OverScene.hpp"

#include <array>


//-----------------------------------------------------------
//シーン切り替えを担当するオブジェクト
//-----------------------------------------------------------
class SceneManager : public GameObject
{
public:

	//コンストラクタ
	//引数：parent	親オブジェクト（基本的にゲームマネージャー）
	SceneManager(GameObject* parent);

	void Init() override;
	void Update() override;
	void Draw() override;
	void Release() override;

    //ゲームに登場するシーン
    enum class SceneID : int
    {
        TEST = 0,
        TITLE,
        PLAY,
        CLEAR,
        OVER,
        MAX_SCENE_ID
    };

	//シーン切り替え（実際に切り替わるのはこの次のフレーム）
	//引数：next	次のシーンのID
	void ChangeScene(SceneID next);

private:

    // 型の配列として扱う
    using SceneList = std::tuple<
        TestScene,
        TitleScene,
        PlayScene,
        ClearScene,
        OverScene
    >;

    // SceneIDとSceneListの対応付け
    template<SceneID SID>
    using SceneType = std::tuple_element_t<
        static_cast<size_t>(SID),
        SceneList
    >;

    template<SceneID SID>
    GameObject* InstantiateByID(GameObject* _pParent)
    {
        return Instantiate<SceneType<SID>>(_pParent);
    }

    using SceneFactoryFn = GameObject*(SceneManager::*)(GameObject*);

    static inline const size_t sceneNum{static_cast<size_t>(SceneID::MAX_SCENE_ID)};
    static const inline std::array<SceneFactoryFn, sceneNum> sceneTable =
    {
        &SceneManager::InstantiateByID<SceneID::TEST>,
        &SceneManager::InstantiateByID<SceneID::TITLE>,
        &SceneManager::InstantiateByID<SceneID::PLAY>,
        &SceneManager::InstantiateByID<SceneID::CLEAR>,
        &SceneManager::InstantiateByID<SceneID::OVER>
    };

    GameObject* InstantiateScene(SceneID _sid,
                                 GameObject* _pParent);

    static const inline SceneID INITIAL_SCENE{SceneID::TEST};

	SceneID currentSceneID_;	//現在のシーン
	SceneID nextSceneID_;		//次のシーン
};
