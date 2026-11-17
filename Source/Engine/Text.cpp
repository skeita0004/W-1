#include "Text.hpp"

#include <cstdlib>

#include "Direct3D.hpp"
#include "CameraSet.hpp"

Text::Text() : hPict_(-1), width_(16), height_(32), fileName_("Picture/char.png"), rowLength_(16)
{
}

Text::~Text()
{
}

//初期化（デフォルト）
HRESULT Text::Initialize()
{
	hPict_ = Image::Load(fileName_);
	assert(hPict_ >= 0);

	if (hPict_ < 0)
		return E_FAIL;

	return S_OK;
}

//初期化（オリジナルの画像）
HRESULT Text::Initialize(const char* fileName, const unsigned int charWidth, const unsigned int charHeight, const unsigned int rowLength)
{
	strcpy_s(fileName_, fileName);
	width_ = charWidth;
	height_ = charHeight;
	rowLength_ = rowLength;
	return Initialize();
}

XMFLOAT2 Text::GetScreenPos(Transform& _t)
{
	XMVECTOR worldPos = XMVector3Transform(XMVectorZero(), _t.GetWorldMatrix());
	XMMATRIX vp = Camera::GetViewMatrix() * Camera::GetProjectionMatrix();

	XMVECTOR clip = XMVector4Transform(XMVectorSetW(worldPos, 1.0f), vp);
	float w = XMVectorGetW(clip);

	float ndcX = XMVectorGetX(clip) / w;
	float ndcY = XMVectorGetY(clip) / w;

	XMFLOAT2 ret{};
	ret.x = (ndcX * 0.5f + 0.5f) * Direct3D::screenWidth_;
	ret.y = (1.0f - (ndcY * 0.5f + 0.5f)) * Direct3D::screenHeight_;

	return ret;
}

void Text::GetScreenPos(float& _x, float& _y)
{
	Transform t{};
	t.position_ = XMFLOAT3(_x, _y, 0.0f);
	XMVECTOR worldPos = XMVector3Transform(XMVectorZero(), t.GetWorldMatrix());
	XMMATRIX vp = Camera::GetViewMatrix() * Camera::GetProjectionMatrix();

	XMVECTOR clip = XMVector4Transform(XMVectorSetW(worldPos, 1.0f), vp);
	float w = XMVectorGetW(clip);

	float ndcX = XMVectorGetX(clip) / w;
	float ndcY = XMVectorGetY(clip) / w;

	_x = (ndcX * 0.5f + 0.5f) * Direct3D::screenWidth_;
	_y = (1.0f - (ndcY * 0.5f + 0.5f)) * Direct3D::screenHeight_;
}

void Text::GetScreenPos(XMFLOAT3& _pos)
{
	GetScreenPos(_pos.x, _pos.y);
}


//描画（文字列）
void Text::Draw(int x, int y, const char* str)
{
	//表示位置（左上）を計算
	//Spriteクラスは中心が(0,0)、右上が(1,1)という座標だが、ここの引数は左上を(0,0)、ドット単位で指定している
	float px, py;

	//引数は左上原点だが、スプライトは画面中央が原点なので、画面サイズの半分ずらす
	px = (float)(x - Direct3D::screenWidth_ / 2);
	py = (float)(-y + Direct3D::screenHeight_ / 2);	//Y軸は+-反転

	//スプライトはPositionを1ずらすと画面サイズの半分ずれるので、ピクセル単位に変換
	px /= (float)(Direct3D::screenWidth_ / 2.0f);
	py /= (float)(Direct3D::screenHeight_ / 2.0f);


	//１文字ずつ表示する
	for (int i = 0; str[i] != '\0'; i++)	//文字列の末尾まで来たら終わり
	{
		//表示したい文字が、画像の何番目に書いてあるかを求める
		int id = str[i] - '!';		//表示したい文字のコードから「!」のコードを引くことで、!＝0、"=1、#＝2･･･という番号にする

		//表示したい文字が、画像のどこにあるかを求める
		int x = id % rowLength_;	//左から何番目
		int y = id / rowLength_;	//上から何番目

		//表示する位置
		Transform transform;
		transform.position_.x = px;
		transform.position_.y = py;

		Image::SetTransform(hPict_, transform);

		//表示する範囲
		Image::SetRect(hPict_, width_ * x, height_ * y, width_, height_);

		//表示
		Image::Draw(hPict_);

		//次の位置にずらす
		px += width_ / (float)(Direct3D::screenWidth_ / 2.0f);
	}
}

//描画（整数値）
void Text::Draw(int x, int y, int value)
{
	//文字列に変換
	char str[256];
	sprintf_s(str, "%d", value);

	Draw(x, y, str);
}

//解放
void Text::Release()
{
	Image::Release(hPict_);
}
