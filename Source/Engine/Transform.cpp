#include "Transform.hpp"

Transform::Transform() :
    pParent(nullptr)
{
	position = XMFLOAT3(0, 0, 0);
	rotate = XMFLOAT3(0, 0, 0);
	scale = XMFLOAT3(1, 1, 1);
}


Transform::~Transform()
{
}

void Transform::Calculation()
{
	//移動行列
	matTranslate = XMMatrixTranslation(position.x, position.y, position.z);

	//回転行列
	XMMATRIX rotateX, rotateY, rotateZ;
	rotateX = XMMatrixRotationX(XMConvertToRadians(rotate.x));
	rotateY = XMMatrixRotationY(XMConvertToRadians(rotate.y));
	rotateZ = XMMatrixRotationZ(XMConvertToRadians(rotate.z));
	matRotate = rotateZ * rotateX * rotateY;

	//拡大縮小
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
}

const XMMATRIX Transform::GetWorldMatrix()
{
	Calculation();
	if (pParent)
	{
		return  matScale * matRotate * matTranslate * pParent->GetWorldMatrix();
	}

	return  matScale * matRotate * matTranslate;
}

const XMMATRIX Transform::GetNormalMatrix()
{
    Calculation();
    if (pParent != nullptr)
    {
        return matRotate * XMMatrixInverse(nullptr, matScale) * pParent->GetNormalMatrix();
    }

    return matRotate * XMMatrixInverse(nullptr, matScale);
}

const XMMATRIX Transform::GetCameraMatrix()
{
    return matRotate * matTranslate;
}

const XMMATRIX Transform::GetTranslateMatrix()
{
    Calculation();
    if (pParent)
    {
        return matTranslate * pParent->GetTranslateMatrix();
    }

    return matTranslate;
}

const XMMATRIX Transform::GetRotateMatrix()
{
    Calculation();
    if (pParent)
    {
        return matRotate * pParent->GetRotateMatrix();
    }
    return matRotate;
}

const XMMATRIX Transform::GetScaleMatrix()
{
    Calculation();
    if (pParent)
    {
        return matScale * pParent->GetScaleMatrix();
    }
    return matScale;
}

