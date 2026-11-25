#include "Fence.hpp"

Fence::Fence(GameObject* parent)
	:Obstacle(parent, "Fence")
{
}

void Fence::Initialize()
{
	hModel_ = Model::Load("Model/HorseRace/HalfFence.fbx");
	assert(hModel_ >= 0);
	transform_.position = XMFLOAT3(0, 0, 1000);

	SetBoxCollider(XMFLOAT3(0.0f, 0.0f, 0.0f), { 200, 200, 25 });
}

void Fence::Update()
{
}

void Fence::Release()
{
}
