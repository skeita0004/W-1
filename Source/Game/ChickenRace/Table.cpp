#include "Table.hpp"

#include "Model.hpp"
#include "InputBinding.hpp"

#include "Bin.hpp"
#include "Beermug.hpp"
#include "Player.hpp"
#include "PlayerAction.hpp"

#include <imgui.h>

Table::Table(GameObject *_pParent) :
	GameObject(_pParent, "Table"),
	hModel_(-1),
	pPlayers_()
{
}

void Table::Initialize()
{
	hModel_ = Model::Load("Model/ChickenRace/table_mock.fbx");

	transform_.scale = XMFLOAT3(7, 7, 7);
	//transform_.position.x = 0.8f;
	transform_.position.y = -2.5;

	for (auto& pPlayer : pPlayers_)
	{
		pPlayer = Instantiate<Player>(this);
	}
	std::map<PlayerAction, InputBinding> pIS1;
	std::map<PlayerAction, InputBinding> pIS2;
	auto& p1 = pPlayers_[0];
	auto& p2 = pPlayers_[1];

	pIS1.insert(std::make_pair(PlayerAction::POUR,
							   InputBinding(DIK_SPACE,
										  Input::UNASSIGNED,
										  Pad(p1->GetPlayerID(), Input::UNASSIGNED))));
	pIS1.insert(std::make_pair(PlayerAction::CONFIRMED,
							   InputBinding(DIK_RETURN,
										  Input::UNASSIGNED,
										  Pad(p1->GetPlayerID(), Input::UNASSIGNED))));

	pIS2.insert(std::make_pair(PlayerAction::POUR,
							   InputBinding(DIK_LSHIFT,
											Input::UNASSIGNED,
											Pad(p2->GetPlayerID(), Input::UNASSIGNED))));
	pIS2.insert(std::make_pair(PlayerAction::CONFIRMED,
							   InputBinding(DIK_RSHIFT,
											Input::UNASSIGNED,
											Pad(p2->GetPlayerID(), Input::UNASSIGNED))));

	p1->SetPosition(XMFLOAT3(-0.6f, 0.1f, 0));
	p1->SetRotate(XMFLOAT3(0, 0, 0));
	p1->InputSet() = pIS1;

	p2->SetPosition(XMFLOAT3(0.6f, 0.1f, 0));
	p2->SetRotate(XMFLOAT3(0, 180, 0));
	p2->InputSet() = pIS2;
}

void Table::Update()
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
}

void Table::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Renderer::Draw(hModel_);
}

void Table::Release()
{
}
