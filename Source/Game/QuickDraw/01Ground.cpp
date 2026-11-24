#include "01Ground.hpp"
#include "Model.hpp"
#include "Input.hpp"

//コンストラクタ
Ground::Ground(GameObject * parent)
	:GameObject(parent, "Ground"), hModel_(-1)
{
}

//デストラクタ
Ground::~Ground()
{
}

//初期化
void Ground::Initialize()
{
	//モデルデータのロード
	hModel_ = Model::Load("Models/QuickDraw/Game01_Bg00.fbx");
	transform_.scale = XMFLOAT3(5.0f, 0.01f, 2.0f);
    transform_.rotate = XMFLOAT3(0.0f, 0.0f, 0.0f);	
	assert(hModel_ >= 0);
}

//更新
void Ground::Update()
{
}

//描画
void Ground::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Renderer::Draw(hModel_);
}

//開放
void Ground::Release()
{
}


