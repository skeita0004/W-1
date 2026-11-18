#pragma once
#include "../../Engine/GameObject.hpp"

class Race : public GameObject {
public:
	Race(GameObject* parent);

    void Init() override;
	void Update() override;
	void Draw() override;
    void Release() override;

private:
};