#pragma once

#include <vector>

#include "Direct3D.hpp"
#include "Vertex.hpp"

struct SkinPartInstance
{
	std::vector<XMMATRIX> diffPose;
	std::vector<Vertex>   vertices;
	ID3D11Buffer*         pVertexBuffer = nullptr;
};

class SkinInstance
{
public:
	SkinInstance() = default;
	~SkinInstance()
	{
		Release();
	}

	void Release();

	std::vector<SkinPartInstance>& Parts()
	{
		return parts_;
	}

	ID3D11Buffer* GetVertexBuffer(size_t _index);

private:
	std::vector<SkinPartInstance> parts_;
};