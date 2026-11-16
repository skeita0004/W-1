#pragma once
#include "Direct3D.hpp"

#include <string>
#include <filesystem>
#include <vector>

import fastgltf;

class GltfParts;

class Gltf
{
public:
    Gltf();
    ~Gltf();

    fastgltf::Error Load(std::filesystem::path _path);

    void Release();

    std::vector<GltfParts*>& Parts()
    {
        return pParts_;
    }

private:
    friend class GltfParts;

    std::vector<GltfParts*> pParts_;
    
    fastgltf::Asset asset_;
    fastgltf::Scene* pGltfScene_;

    void CheckNode(fastgltf::Node* _pNode, std::vector<GltfParts*>& _pParts);
};