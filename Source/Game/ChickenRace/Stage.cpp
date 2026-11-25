#include "Stage.hpp"

#include "Table.hpp"

#include "CameraSet.hpp"

#include <imgui.h>

Stage::Stage(GameObject *_pParent) :
	GameObject(_pParent, "Stage"),
	pTable_(nullptr)
{
}

void Stage::Initialize()
{
	pTable_ = Instantiate<Table>(this);
}

void Stage::Update()
{
#ifdef _DEBUG
	std::string panelName{"ObjectTS" + objectName_};

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

	CameraSet::GetCurrent()->GetTargetTransform().position = XMFLOAT3(0, 0, 0);
}

void Stage::Draw()
{
}

void Stage::Release()
{
}
