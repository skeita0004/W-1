#include "Obstacle.hpp"
#include "Model.hpp"

Obstacle::Obstacle(GameObject* parent, const char* name)
	:GameObject(parent, name), hModel_(-1)
{  
}

void Obstacle::Initialize()
{
}

void Obstacle::Update()
{
}

void Obstacle::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Renderer::Draw(hModel_);

	CollisionDraw();
}

void Obstacle::Release()
{
}

void Obstacle::SetPosition(XMFLOAT3 pos)
{
	transform_.position = pos;
}

void Obstacle::SetBoxCollider(XMFLOAT3 center, XMFLOAT3 size)
{
	AddCollider(new BoxCollider(center, size));
}