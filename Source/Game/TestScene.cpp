#include "TestScene.hpp"
#include "SceneManager.hpp"
#include "Input.hpp"
#include "Model.hpp"
#include "Renderer.hpp"

TestScene::TestScene(GameObject * parent) : 
    GameObject(parent, "TestScene"),
    pSceneManager_(nullptr)
{
}

void TestScene::Init()
{
    pSceneManager_ = FindObject<SceneManager>("SceneManager");
    hModel_ = Model::Load("Models/mochi_kinchaku.fbx");
    //hModel_ = Model::Load("Models/ps_enemy.fbx");

    EffekseerLib::gEfk->AddEffect("BLUE_LASER", "Effects/AndrewFM01/blue_laser.efk");

    Model::SetAnimFrame(hModel_, 1, 60, 1.0f);
}

void TestScene::Update()
{
    if (Input::IsKey(DIK_D))
    {
        transform.rotate.y -= 2.0f;
    }

    if (Input::IsKeyDown(DIK_SPACE))
    {
        EffekseerLib::EFKTransform t;
        XMStoreFloat4x4(&(t.matrix), transform.GetWorldMatrix());
        t.isLoop = false;
        t.maxFrame = 300;
        t.speed = 2.0f;
        effectTsfrm = EffekseerLib::gEfk->Play("BLUE_LASER", t);
    }
    
    if (effectTsfrm)
    {
        XMStoreFloat4x4(&effectTsfrm->matrix, transform.GetWorldMatrix());
    }

    transform.position.y = -5.f;

    float sc = 2.0f;
    transform.scale = XMFLOAT3(sc, sc, sc);

    Model::SetTransform(hModel_, transform);

    //Model::Update(hModel_);

    // シーン遷移を追加する
}

void TestScene::Draw()
{
    Renderer::Draw(hModel_);
   
}

void TestScene::Release()
{
}
