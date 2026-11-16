#include "GltfParts.hpp"

GltfParts::GltfParts()
{
}

GltfParts::~GltfParts()
{
}

HRESULT GltfParts::Init(const fastgltf::Mesh& _mesh, const fastgltf::Asset& _asset)
{
    InitVertex(_mesh, _asset);
    InitMaterial(_mesh, _asset);
    InitIndex(_mesh, _asset);
    
    return S_OK;
}

void GltfParts::InitVertex(const fastgltf::Mesh& _mesh, const fastgltf::Asset& _asset)
{
    for (auto& primitive : _mesh.primitives)
    {
        auto posIt = primitive.findAttribute("POSITION");

        for (posIt; posIt != primitive.attributes.end(); ++posIt)
        {
            const auto& accessor = _asset.accessors[posIt->accessorIndex];
            vertexCount_ += accessor.count;
        }
    }

    D3D11_BUFFER_DESC bdVertex{};
    bdVertex.ByteWidth = sizeof(Vertex) * vertexCount_;
    bdVertex.Usage = D3D11_USAGE_DYNAMIC;
    bdVertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bdVertex.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bdVertex.MiscFlags = 0;
    bdVertex.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA srDataVertex{};
    srDataVertex.pSysMem = pVertexData_.data();
    Direct3D::pDevice_->CreateBuffer(&bdVertex, &srDataVertex, &pVertexBuffer_);
}

void GltfParts::InitMaterial(const fastgltf::Mesh& _mesh, const fastgltf::Asset& _asset)
{
}

void GltfParts::InitTexture(const fastgltf::Mesh& _mesh, const fastgltf::Asset& _asset)
{
}

void GltfParts::InitIndex(const fastgltf::Mesh& _mesh, const fastgltf::Asset& _asset)
{


}
