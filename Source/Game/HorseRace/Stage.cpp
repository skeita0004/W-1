#include "Stage.hpp"
#include "Model.hpp"
#include "csvReader.hpp"

#include "Fence.hpp"
#include "Rock.hpp"
#include "cactus1.hpp"
#include "cactus2.hpp"


Stage::Stage(GameObject* parent)
	:GameObject(parent, "Stage"), hModel_(-1)
{
}

void Stage::Initialize()
{
	hModel_ = Model::Load("Model/HorseRace/Game04_StageFloor00.fbx");
	//assert(hModel_ >= 0);
	transform_.position = XMFLOAT3(0, 0, 0);
	transform_.scale = XMFLOAT3(1, 1, 50);

	horse = Instantiate<Horse>(GetParent());

	LoadStageCsv();
}

void Stage::Update()
{
    for (auto& obstacle : obs)
    {
        if (obstacle != nullptr)
        {
            horse->Collision(obstacle);
        }
    }
}

void Stage::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Renderer::Draw(hModel_);
}

void Stage::Release()
{
}

void Stage::LoadStageCsv()
{
    CsvReader csv("Data/Stage.csv");

    const float xPositions[4] =
    {
        -300.0f,
        -100.0f,
         100.0f,
         300.0f
    };

    for (int i = 0; i < 15; i++)
    {
		static const int numColumns = 4;
        for (int j = 0; j < numColumns; j++)
        {
            int type = csv.GetInt(i, j);

            if (type == 0)
                continue;

            Obstacle* obstacle = nullptr;

            switch (type)
            {
            case 1:
                obstacle = Instantiate<Fence>(GetParent());
                break;

            case 2:
                obstacle = Instantiate<Rock>(GetParent());
                break;

            case 3:
                obstacle = Instantiate<cactus1>(GetParent());
                break;

            case 4:
                //obstacle = Instantiate<cactus2>(GetParent());
                break;
            }

            if (obstacle != nullptr)
            {
                float x = xPositions[j];
                float z = (i + 1) * 1500.0f;

                obstacle->SetPosition(XMFLOAT3(x, 0, z));

                obs.push_back(obstacle);
            }
        }
    }
}