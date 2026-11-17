#include "ChickenRaceScene.h"

#include "Stage.h"
#include "BackGround.h"
#include "Judge.h"

#include "./Engine/Camera.h"
#include "Engine/SceneManager.h"

ChickenRaceScene::ChickenRaceScene(GameObject *_pParent) :
	GameObject(_pParent, "ChickenRaceScene"),
	pSceneManager_(nullptr),
	pJudge_(nullptr)
{
}

void ChickenRaceScene::Initialize()
{
	pSceneManager_ = static_cast<SceneManager*>(FindObject("SceneManager"));

	XMFLOAT3 camPos{0.f, 0.f, -10.f};
	Camera::SetPosition(camPos);

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
