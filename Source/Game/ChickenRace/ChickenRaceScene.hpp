#pragma once

#include "../Engine/GameObject.h"

class SceneManager;
class Judge;

class ChickenRaceScene : public GameObject
{
public:
	ChickenRaceScene(GameObject *_pParent);

	void Initialize() override;
	void Update()     override;
	void Draw()       override;
	void Release()    override;

private:
	SceneManager *pSceneManager_;
	Judge *pJudge_;
};