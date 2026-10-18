#include "TestScene.hpp"
#include "SceneManager.hpp"
#include "Input.hpp"

TestScene::TestScene(GameObject * parent) : 
    GameObject(parent, "TestScene"),
    pSceneManager_(nullptr)
{
}

void TestScene::Init()
{
    pSceneManager_ = FindObject<SceneManager>("SceneManager");
}

void TestScene::Update()
{
    // シーン遷移を追加する
}

void TestScene::Draw()
{
}

void TestScene::Release()
{
}
