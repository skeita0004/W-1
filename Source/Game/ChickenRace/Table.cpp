#include "Table.h"

#include "Engine/Model.h"
#include "Engine/InputBinding.h"

#include "Bin.h"
#include "Beermug.h"
#include "Player.h"
#include "PlayerAction.h"

Table::Table(GameObject *_pParent) :
	GameObject(_pParent, "Table"),
	hModel_(-1),
	pPlayers_()
{
}

void Table::Initialize()
{
	hModel_ = Model::Load("Model/table_mock.fbx");

	transform_.scale_ = XMFLOAT3(7, 7, 7);
	transform_.position_.y = -3;

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
										  Input::ANASSIGNED,
										  Pad(p1->GetPlayerID(), Input::ANASSIGNED))));
	pIS1.insert(std::make_pair(PlayerAction::CONFIRMED,
							   InputBinding(DIK_RETURN,
										  Input::ANASSIGNED,
										  Pad(p1->GetPlayerID(), Input::ANASSIGNED))));

	pIS2.insert(std::make_pair(PlayerAction::POUR,
							   InputBinding(DIK_LSHIFT,
											Input::ANASSIGNED,
											Pad(p2->GetPlayerID(), Input::ANASSIGNED))));
	pIS2.insert(std::make_pair(PlayerAction::CONFIRMED,
							   InputBinding(DIK_RSHIFT,
											Input::ANASSIGNED,
											Pad(p2->GetPlayerID(), Input::ANASSIGNED))));

	p1->SetPosition(XMFLOAT3(-0.55f, 0.1, 0));
	p1->SetRotate(XMFLOAT3(0, 0, 0));
	p1->InputSet() = pIS1;

	p2->SetPosition(XMFLOAT3(0.55f, 0.1, 0));
	p2->SetRotate(XMFLOAT3(0, 180, 0));
	p2->InputSet() = pIS2;
}

void Table::Update()
{

}

void Table::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Table::Release()
{
}
