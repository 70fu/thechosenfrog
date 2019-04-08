
#include "AssetList.h"
#include "../RuntimeClasses.h"
#include <ObjectInterfacePerModule.h>

class ShaderProgramList : public TInterface<RuntimeClassIds::SHADER_PROGRAM_LIST,AssetList<ShaderProgramAsset,AssetType::SHADER_PROGRAM>>
{
protected:
    void loadAssets(ShaderProgramAsset *assets, size_t size, AssetManager& assetManager) override
    {
        //loadAssetsFromFileHelper("/music/error.ogg",ShaderIds::DEFAULT,assets[ShaderIds::DEFAULT],assetManager);
        //...
    }

    bool loadAssetFromFile(const std::string &path, ShaderProgramAsset &asset) override
    {
        return false;
    }

    void loadDefault(ShaderProgramAsset &asset, AssetManager& assetManager) override
    {
    }
};
REGISTERCLASS(ShaderProgramList);
