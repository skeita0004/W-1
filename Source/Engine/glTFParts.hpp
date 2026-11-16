#pragma once

#include "Direct3D.hpp"
#include "Material.hpp"
#include "Vertex.hpp"

import fastgltf;

class Fbx;

class GltfParts
{
public:
	GltfParts();
	~GltfParts();

	HRESULT Init(const fastgltf::Mesh& _mesh, const fastgltf::Asset& _asset);

private:
	size_t vertexCount_;
	size_t polygonCount_;
	std::vector<size_t> indexCount_;
	size_t materialCount_;
	size_t polygonVertexCount_;

	std::vector<Vertex*> pVertexData_;
	std::vector<std::vector<size_t>> indexData_;

	ID3D11Buffer* pVertexBuffer_;
	std::vector<ID3D11Buffer*> pIndexBuffers_;
	std::vector<Material> materialList_;

	void InitVertex(const fastgltf::Mesh& _mesh, const fastgltf::Asset& _asset);
	void InitMaterial(const fastgltf::Mesh& _mesh, const fastgltf::Asset& _asset);
	void InitTexture(const fastgltf::Mesh& _mesh, const fastgltf::Asset& _asset);
	void InitIndex(const fastgltf::Mesh& _mesh, const fastgltf::Asset& _asset);
};