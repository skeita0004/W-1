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
}

void TestScene::Update()
{
    if (Input::IsKey(DIK_D))
    {
        transform.rotate.y -= 1.0f;
    }

    //transform.position.y = -5.f;

    float sc = 3.0f;
    transform.scale = XMFLOAT3(sc, sc, sc);


    // シーン遷移を追加する
}

void TestScene::Draw()
{

    Model::SetTransform(hModel_, transform);
    Renderer::Draw(hModel_);
   
}

void TestScene::Release()
{
}
