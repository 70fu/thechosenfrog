#include "AssetList.h"
#include "../RuntimeClasses.h"
#include <ObjectInterfacePerModule.h>
#include "Assets.h"
#include "Material.h"
#include "MaterialIds.h"
#include "AssetManager.h"

class MaterialList : public TInterface<RuntimeClassIds::MATERIAL_LIST,AssetList<MaterialAsset ,AssetType::MATERIAL>>
{
protected:
    void loadAssets(MaterialAsset *assets, size_t size, AssetManager& assetManager) override
    {
        loadDefault(assets[MaterialIds::DEFAULT],assetManager);
    }

    bool loadAssetFromFile(const std::string &path, MaterialAsset &asset) override
    {
        return false;//currently not supported
    }

    void loadDefault(MaterialAsset &asset, AssetManager& assetManager) override
    {
        asset.data.clearProps();
        asset.shader = assetManager.getShaderProgram(ShaderProgramIds::DEFAULT);
    }
};
REGISTERCLASS(MaterialList);
