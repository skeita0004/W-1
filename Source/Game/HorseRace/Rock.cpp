#include "Rock.hpp"

Rock::Rock(GameObject* parent)
	:Obstacle(parent, "Rock")
{
}

void Rock::Initialize()
{
	hModel_ = Model::Load("Model/HorseRace/Rock00.fbx");

	SetBoxCollider(XMFLOAT3(0.0f, 0.0f, 0.0f), { 200, 400, 200 });
}

void Rock::Update()
{
}

void Rock::Release()
{
}
