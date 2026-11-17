#pragma once

#include "../Engine/GameObject.h"

class SceneManager;
class TitleMessage;

class TitleScene : public GameObject
{
public:
	TitleScene(GameObject* _pParent);

	void Initialize() override;
	void Update()     override;
	void Draw()       override;
	void Release()    override;

private:
	SceneManager* pSceneManager_;

	TitleMessage* pTitleMessage_;

	bool isEnterSceneChange_;
};