#pragma once
#include "GameObject.hpp"

class SceneManager;

class TitleScene : public GameObject
{
public:
    TitleScene(GameObject* _pParent);

    void Init() override;
    void Update() override;
    void Draw() override;
    void Release() override;

private:
    SceneManager* pSceneManager_;
    int hImage_;
    int hStartImage_;
};