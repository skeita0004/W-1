#include "RaceScene.hpp"
#include "Audio.hpp"
#include "Stage.hpp"
#include "Image.hpp"
#include "SceneManager.hpp"

#include "SpeedLine.hpp"

RaceScene::RaceScene(GameObject* parent)
	:GameObject(parent, "RaceScene"), hImage_(-1)
{
}

void RaceScene::Initialize()
{
	hImage_ = Image::Load("Picture/HorseRace/yuuhi.jpg");
	//hImageS_ = Image::Load("Picture/HorseRace/SpeedLine.png");
	hBgm_ = Audio::Load("Audio/HorseRace/HorseBgm.wav");

	Instantiate<Stage>(this);

	speedLine_ = Instantiate<SpeedLine>(this);
	speedLine_->DisableDraw();

	horse = (Horse*)FindChildObject("Horse");
}

void RaceScene::Update()
{
	Audio::Play(hBgm_);
	
	if (horse == nullptr)
		return;

	

	XMFLOAT3 pPos = horse->GetPosition();
	if (pPos.z >= 25000.0f)
	{
		SceneManager* sceneManager = (SceneManager*)GetParent();
		sceneManager->ChangeScene(SceneManager::SceneID::RESULT);
	}
}

void RaceScene::Draw()
{
	if (horse->GetSpeed() >= 12.0f)
	{
		speedLine_->EnableDraw();
	}

	//Image::SetTransform(hImage_, transform_);
	//Image::Draw(hImage_);
}

void RaceScene::Release()
{
}
