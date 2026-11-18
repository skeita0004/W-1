#include "Tumbleweed.hpp"

Tumbleweed::Tumbleweed(GameObject* parent)
	:Obstacle(parent, "Tumbleweed")
{
}

void Tumbleweed::Initialize()
{
	hModel_ = Model::Load("Model/Tumbleweed00.fbx");
}

void Tumbleweed::Update()
{
}

void Tumbleweed::Release()
{
}
