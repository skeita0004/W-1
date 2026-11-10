#include "Gltf.hpp"

Gltf::Gltf()
{
}

Gltf::~Gltf()
{
}

fastgltf::Error Gltf::Load(std::filesystem::path _path)
{
    fastgltf::Parser parser(fastgltf::Extensions::None);

    // gltf, GLBをメモリ上に展開する
    auto gltfFile = fastgltf::GltfDataBuffer::FromPath(_path);
    if (not(bool(gltfFile)))
    {
        return gltfFile.error();
    }

    // メモリ上に展開されたファイルを解釈する
    auto gltfAsset = parser.loadGltf(gltfFile.get(), _path.parent_path());
    auto err = gltfAsset.error();
    if (err != fastgltf::Error::None)
    {
        return err;
    }
    asset_ = std::move(gltfAsset.get());

    for (auto& parts : gltfAsset.get().meshes)

    return fastgltf::Error::None;
}

void Gltf::Release()
{
}
