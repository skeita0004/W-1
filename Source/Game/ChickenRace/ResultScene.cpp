#include "ResultScene.h"

#include "Engine/SceneManager.h"
#include "Engine/Text.h"
#include "Engine/Input.h"

#include "Judge.h"

ResultScene::ResultScene(GameObject *_pParent) :
	GameObject(_pParent, "ResultScene"),
	pSceneManager_(),
	pText_(nullptr)
{
}

void ResultScene::Initialize()
{
	pSceneManager_ = static_cast<SceneManager *>(FindObject("SceneManager"));

	pText_ = new Text();
	pText_->Initialize();
}

void ResultScene::Update()
{

	for (size_t i = 0; i < Input::MAX_PAD_NUM; i++)
	{
		if (Input::IsPadButtonDown(XINPUT_GAMEPAD_A, i) or
			Input::IsKeyDown(DIK_SPACE))
		{
			pSceneManager_->ChangeScene(SCENE_ID_TITLE);
		}
	}
}

void ResultScene::Draw()
{
	std::string resultMessage{ "Winner : " + Judge::winnerName };

	int offset = 16 * resultMessage.length();
	pText_->Draw((Direct3D::screenWidth_ * 0.5f) - offset * 0.5f,
				  Direct3D::screenHeight_ * 0.5f,
				  resultMessage.c_str());

	std::string str{ "Reason:" };
	offset = 16 * str.length();
	pText_->Draw( (Direct3D::screenWidth_ * 0.5f ) - offset * 0.5f,
				  450,
				  str.c_str());

	offset = 16 * Judge::reason.length();
	pText_->Draw( (Direct3D::screenWidth_ * 0.5f) - offset * 0.5f,
				  480,
				  Judge::reason.c_str() );

	if (Judge::jr != Judge::WATER_OVERFLOW)
	{
		std::string p1WaterLevel{ "p1 : " + std::to_string(Judge::p1WaterLevel) + "ml"};
		std::string p2WaterLevel{ "p2 : " + std::to_string( Judge::p2WaterLevel ) + "ml" };
		std::string waterLevel{ p1WaterLevel + "  |  " + p2WaterLevel};

		offset = 16 * waterLevel.length();
		pText_->Draw((Direct3D::screenWidth_ * 0.5f) - offset * 0.5f,
					 540,
					 waterLevel.c_str());
	}
}

void ResultScene::Release()
{
}
