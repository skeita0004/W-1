#include "RootObject.hpp"
#include "SceneManager.hpp"


RootObject::RootObject():
	GameObject(nullptr, "RootObject")
{
}


RootObject::~RootObject()
{
}

void RootObject::Initialize()
{
	Instantiate<SceneManager>(this);
}

void RootObject::Update()
{
}

void RootObject::Draw()
{
}

void RootObject::Release()
{
}

void RootObject::DrawAll(int _viewportNum)
{
	for (int i = 0; i < _viewportNum; i++)
	{

	}
}
