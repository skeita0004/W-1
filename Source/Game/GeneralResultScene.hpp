#pragma once

#include "GameObject.hpp"
#include "FontText.hpp"

class GeneralResultScene : public GameObject
{
public:
    GeneralResultScene(GameObject* parent);

    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

private:
    FontText text_;
};