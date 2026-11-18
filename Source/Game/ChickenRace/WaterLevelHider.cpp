#include "WaterLevelHider.hpp"

#include "Image.hpp"
#include "Model.hpp"

#include <imgui.h>

WaterLevelHider::WaterLevelHider(GameObject *_pParent) :
	GameObject(_pParent, "WaterLevelHider"),
	hImage_(-1)
{
}

void WaterLevelHider::Initialize()
{
	hImage_ = Model::Load("Model/question_mark.fbx");
	SetScale(2.25f, 2.25f, 1.0f);
}

void WaterLevelHider::Update()
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

void WaterLevelHider::Draw()
{
	Model::SetTransform(hImage_, transform_);
	Renderer::Draw(hImage_);
}

void WaterLevelHider::Release()
{
}
