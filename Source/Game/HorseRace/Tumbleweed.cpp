#include "Tumbleweed.hpp"

Tumbleweed::Tumbleweed(GameObject* parent)
	:Obstacle(parent, "Tumbleweed")
{
}

void Tumbleweed::Initialize()
{
	hModel_ = Model::Load("Model/HorseRace/Tumbleweed01.fbx");
    transform_.scale = XMFLOAT3(10.0f, 10.0f, 10.0f);
	SetBoxCollider(XMFLOAT3(0.0f, 0.0f, 0.0f), {200, 400, 200});
}

void Tumbleweed::Update()
{
}

void Tumbleweed::Release()
{
}
