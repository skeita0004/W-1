#pragma once
#include <DirectXMath.h>

struct Vertex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT3 uv;
	DirectX::XMFLOAT3 tangent;
	DirectX::XMFLOAT3 binormal;
};