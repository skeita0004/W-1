
#pragma once

#include "Engine/GameObject.h"
#include <string>

class Game01Player : public GameObject
{
public:
    Game01Player(
        GameObject* parent,
        const std::string& modelPath,
        float posX,
        float rotateY
    );

    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    //倒れた状態にする
    void SetFallen(bool fallen);

private:
    int hModel_ = -1;
    std::string modelPath_;
    float defaultRotateY_ = 0.0f;
    bool isFallen_ = false;
};
