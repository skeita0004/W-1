#pragma once

#include "../Engine/GameObject.h"

class SceneManager;
class Text;

class ResultScene : public GameObject
{
public:
	ResultScene(GameObject *_pParent);

	void Initialize() override;
	void Update()     override;
	void Draw()       override;
	void Release()    override;

private:
	SceneManager *pSceneManager_;
	Text *pText_;
};