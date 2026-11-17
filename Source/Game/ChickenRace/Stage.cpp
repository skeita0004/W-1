#include "Stage.h"

#include "Table.h"

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
}

void Stage::Draw()
{
}

void Stage::Release()
{
}
