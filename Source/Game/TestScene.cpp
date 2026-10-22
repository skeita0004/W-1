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
    hModel_ = Model::Load("Models/Player_cube.fbx");
}

void TestScene::Update()
{
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
