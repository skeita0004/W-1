#include "FallingRock.hpp"

FallingRock::FallingRock(GameObject* parent)
	:Obstacle(parent, "FallingRock")
{
}

void FallingRock::Initialize()
{
	hModel_ = Model::Load("Assets/Model/Rock01.fbx");
}

void FallingRock::Update()
{
}

void FallingRock::Release()
{
}
