#include "TestScene.hpp"

#include "Model.hpp"

#include <imgui.h>

TestScene::TestScene(GameObject* _pParent) :
	GameObject(_pParent, "TestScene"),
	hModel_(-1)
{
}

void TestScene::Initialize()
{
	hModel_ = Model::Load("model/oden.fbx");

	//transform_.position.y = -2.f;
}

void TestScene::Update()
{
#ifdef _DEBUG
	ImGui::Begin("Odenform");
	ImGui::InputFloat("pos.x", &transform_.position.x);
	ImGui::InputFloat("pos.y", &transform_.position.y);
	ImGui::InputFloat("pos.z", &transform_.position.z);

	ImGui::InputFloat("rot.x", &transform_.rotate.x);
	ImGui::InputFloat("rot.y", &transform_.rotate.y);
	ImGui::InputFloat("rot.z", &transform_.rotate.z);

	ImGui::InputFloat("scl.x", &transform_.scale.x);
	ImGui::InputFloat("scl.y", &transform_.scale.y);
	ImGui::InputFloat("scl.z", &transform_.scale.z);
	ImGui::End();
#endif
}

void TestScene::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Renderer::Draw(hModel_);
}

void TestScene::Release()
{
}
