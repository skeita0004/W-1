#include "cactus2.hpp"

cactus2::cactus2(GameObject* parent)
	:Obstacle(parent, "cactus2")
{
}

void cactus2::Initialize()
{
	hModel_ = Model::Load("Model/cactus01.fbx");

	SetBoxCollider(XMFLOAT3(0.0f, 0.0f, 0.0f), { 200, 200, 100 });
}

void cactus2::Update()
{
}

void cactus2::Release()
{
}
