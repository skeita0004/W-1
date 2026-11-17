#include "WaterLevelHider.h"

#include "Engine/Image.h"
#include "Engine/Model.h"

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
	ImGui::InputFloat("pos.x", &transform_.position_.x);
	ImGui::InputFloat("pos.y", &transform_.position_.y);
	ImGui::InputFloat("pos.z", &transform_.position_.z);

	ImGui::InputFloat("rot.x", &transform_.rotate_.x);
	ImGui::InputFloat("rot.y", &transform_.rotate_.y);
	ImGui::InputFloat("rot.z", &transform_.rotate_.z);

	ImGui::InputFloat("scl.x", &transform_.scale_.x);
	ImGui::InputFloat("scl.y", &transform_.scale_.y);
	ImGui::InputFloat("scl.z", &transform_.scale_.z);
	ImGui::End();
#endif
}

void WaterLevelHider::Draw()
{
	Model::SetTransform(hImage_, transform_);
	Model::Draw(hImage_);
}

void WaterLevelHider::Release()
{
}
