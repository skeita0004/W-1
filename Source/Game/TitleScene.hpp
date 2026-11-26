#pragma once

#include "GameObject.hpp"
#include "FontText.hpp"

class SceneManager;

class TitleScene : public GameObject
{
public:
    TitleScene(GameObject* parent);

    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

private:
    SceneManager* sceneManager_;

    FontText titleText_;
    FontText pushText_;
};