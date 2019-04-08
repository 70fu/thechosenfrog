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
        //TODO
    }

    bool loadAssetFromFile(const std::string &path, MaterialAsset &asset) override
    {
        return false;
    }

    void loadDefault(MaterialAsset &asset, AssetManager& assetManager) override
    {
        //TODO
    }
};
REGISTERCLASS(MaterialList);