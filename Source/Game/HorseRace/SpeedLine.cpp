#include "Speedline.hpp"
#include "Image.hpp"

SpeedLine::SpeedLine(GameObject* parent)
	:GameObject(parent, "SpeedLine")
{
}

SpeedLine::~SpeedLine()
{
}

void SpeedLine::Initialize()
{
	hImage_ = Image::Load("Picture/HorseRace/SpeedLine.png");
}

void SpeedLine::Update()
{
}

void SpeedLine::Draw()
{
	Image::SetTransform(hImage_, transform_);
	Image::Draw(hImage_);
}

void SpeedLine::Release()
{
}
