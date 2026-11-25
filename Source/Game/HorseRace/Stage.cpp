#include "Stage.hpp"
#include "Model.hpp"
#include "csvReader.hpp"

#include "Fence.hpp"
#include "Rock.hpp"
#include "cactus1.hpp"
#include "cactus2.hpp"
#include "Tumbleweed.hpp"


Stage::Stage(GameObject* parent)
	:GameObject(parent, "Stage"), hModel_(-1)
{
}

void Stage::Initialize()
{
	hModel_ = Model::Load("Model/HorseRace/floor.fbx");
	//assert(hModel_ >= 0);
	transform_.position = XMFLOAT3(0, 0, 0);
	transform_.scale = XMFLOAT3(2, 1, 2);

	horse = Instantiate<Horse>(GetParent());

    for (int z = 0; z < lengthCount_; z++)
    {
        for (int x = 0; x < widthCount_; x++)
        {
            float posX = (x - (widthCount_ - 1) / 2.0f) * tileSize_;
            float posZ = z * tileSize_;

            floors.push_back(
                XMFLOAT3(posX, 0.0f, posZ)
            );
        }
    }

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
    float horseZ = horse->GetPosition().z;

    for (const auto& pos : floors)
    {
        // 馬から前後3000以上離れていたら描画しない
        if (fabs(pos.z - horseZ) > 6000.0f)
            continue;

        transform_.position = pos;
        Model::SetTransform(hModel_,transform_);

        Renderer::Draw(hModel_);
    }
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
            case 5:
                obstacle = Instantiate<Tumbleweed>(GetParent());
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