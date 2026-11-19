#include "Infomation.h"
#include "Engine/Image.h"

//コンストラクタ
Infomation::Infomation(GameObject * parent)
	:GameObject(parent, "Infomation"), hPict_(-1)
{
	//画像データのロード
	hPict_ = Image::Load("Picture/info.png");
	assert(hPict_ >= 0);

	//位置
	transform_.position_ = XMFLOAT3(-0.75f, 0.8f, 0.0f);
}

//デストラクタ
Infomation::~Infomation()
{
}

//初期化
void Infomation::Initialize()
{

}

//更新
void Infomation::Update()
{
}

//描画
void Infomation::Draw()
{
	Image::SetTransform(hPict_, transform_);
	Image::Draw(hPict_);
}

//開放
void Infomation::Release()
{
}


