#include "ChickenRaceScene.hpp"

#include "Stage.hpp"
#include "BackGround.hpp"
#include "Judge.hpp"

#include "CameraSet.hpp"
#include "SceneManager.hpp"

#include <imgui.h>

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
	CameraSet::GetVirtualCamera(CameraSet::hNormalCamera)->GetTransform().position = camPos;

	Instantiate<BackGround>(this);
	Instantiate<Stage>(this);

	pJudge_ = Instantiate<Judge>(this);
}

void ChickenRaceScene::Update()
{
#ifdef _DEBUG
	std::string panelName{"ObjectTS : " + objectName_};

	ImGui::Begin(panelName.c_str());
	ImGui::InputFloat("pos.x", &transform_.position.x);
	ImGui::InputFloat("pos.y", &transform_.position.y);
	ImGui::InputFloat("pos.z", &transform_.position.z);

	ImGui::InputFloat("rot.x", &transform_.rotate.x);
	ImGui::InputFloat("rot.y", &transform_.rotate.y);
	ImGui::InputFloat("rot.z", &transform_.rotate.z);

	ImGui::InputFloat("scl.x", &transform_.scale.x);
	ImGui::InputFloat("scl.y", &transform_.scale.y);
	ImGui::InputFloat("scl.z", &transform_.scale.z);
	ImGui::End();
#endif

	if (Judge::confirmedCount == 2)
	{
		pJudge_->Judgement();
		static int changeCounter{0};
		changeCounter++;

		int changeSecond{ 3 };

		if (changeCounter % (60 * changeSecond) == 0)
		{
			//pSceneManager_->ChangeScene(SCENE_ID_RESULT);
		}
	}
}

void ChickenRaceScene::Draw()
{
}

void ChickenRaceScene::Release()
{
}
