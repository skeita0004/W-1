#pragma once
#include "GameObject.hpp"

class SceneManager;

class OverScene : public GameObject
{
public:
    OverScene(GameObject* _pParent);

    void Init() override;
    void Update() override;
    void Draw() override;
    void Release() override;

private:
    SceneManager* pSceneManager_;
};