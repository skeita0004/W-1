#include "ChickenRaceScene.hpp"

#include "Stage.hpp"
#include "BackGround.hpp"
#include "Judge.hpp"

#include "CameraSet.hpp"
#include "SceneManager.hpp"

ChickenRaceScene::ChickenRaceScene(GameObject *_pParent) :
	GameObject(_pParent, "ChickenRaceScene"),
	pSceneManager_(nullptr),
	pJudge_(nullptr)
{
}

void ChickenRaceScene::Initialize()
{
	pSceneManager_ = (FindObject<SceneManager>("SceneManager"));

	XMFLOAT3 camPos{0.f, 0.f, -10.f};
	CameraSet cs{};
	cs.GetVirtualCamera(CameraSet::hNormalCamera)->GetTransform().position = camPos;

	Instantiate<BackGround>(this);
	Instantiate<Stage>(this);

	pJudge_ = Instantiate<Judge>(this);
}

void ChickenRaceScene::Update()
{
	if (Judge::confirmedCount == 2)
	{
		pJudge_->Judgement();
		static int changeCounter{0};
		changeCounter++;

		int changeSecond{ 3 };

		if (changeCounter % (60 * changeSecond) == 0)
		{
			pSceneManager_->ChangeScene(SCENE_ID_RESULT);
		}
	}
}

void ChickenRaceScene::Draw()
{
}

void ChickenRaceScene::Release()
{
}
