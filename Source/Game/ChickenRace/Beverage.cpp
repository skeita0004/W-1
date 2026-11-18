#include "Beverage.hpp"

#include "Model.hpp"

#include <imgui.h>

Beverage::Beverage(GameObject *_pParent) :
	GameObject(_pParent, "Beverage"),
	hModel_(-1)
{
}

void Beverage::Initialize()
{
	hModel_ = Model::Load("Model/beverage.fbx");
}

void Beverage::Update()
{
#ifdef _DEBUG
	std::string panelName{ "ObjectTS" + objectName_ };

	ImGui::Begin(panelName.c_str());
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

void Beverage::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Renderer::Draw(hModel_);
}

void Beverage::Release()
{
}
