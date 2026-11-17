#include "Beverage.h"

#include "Engine/Model.h"

#include "ImGui/imgui.h"

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

void Beverage::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Beverage::Release()
{
}
