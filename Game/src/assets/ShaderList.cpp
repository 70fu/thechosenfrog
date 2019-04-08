#include "AssetList.h"
#include "../RuntimeClasses.h"
#include <ObjectInterfacePerModule.h>

class ShaderList : public TInterface<RuntimeClassIds::SHADER_LIST,AssetList<ShaderAsset,AssetType::SHADER>>
{
protected:
    void loadAssets(ShaderAsset *assets, size_t size, AssetManager& assetManager) override
    {
        //loadAssetsFromFileHelper("/music/error.ogg",ShaderIds::DEFAULT,assets[ShaderIds::DEFAULT],assetManager);
        //...
        //TODO
    }

    bool loadAssetFromFile(const std::string &path, ShaderAsset &asset) override
    {
        //TODO
        return false;
    }

    void loadDefault(ShaderAsset &asset, AssetManager& assetManager) override
    {
        //TODO
    }
};
REGISTERCLASS(ShaderList);
