#include "SkinInstance.hpp"
#include "SafeCleaning.hpp"

void SkinInstance::Release()
{
	for (auto& parts : parts_)
	{
		SafeCleaning::SafeRelease<ID3D11Buffer>(parts.pVertexBuffer);
	}
	parts_.clear();
}

ID3D11Buffer* SkinInstance::GetVertexBuffer(size_t _index)
{
	if (_index > parts_.size())
	{
		return nullptr;
	}
	
	return parts_[_index].pVertexBuffer;
}
