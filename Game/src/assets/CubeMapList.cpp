#include "AssetList.h"
#include "../RuntimeClasses.h"
#include <ObjectInterfacePerModule.h>
#include "Assets.h"
#include "CubeMap.h"
#include "AssetManager.h"

class CubeMapList : public TInterface<RuntimeClassIds::CUBE_MAP_LIST,AssetList<CubeMapAsset ,AssetType::CUBE_MAP>>
{
protected:
    void loadAssets(CubeMapAsset *assets, size_t size, AssetManager& assetManager) override
    {
        loadDefault(assets[CubeMapIds::DEFAULT],assetManager);
        //TODO register paths in asset manager correctly
        //...
    }

    bool loadAssetFromFile(const std::string &path, CubeMapAsset &asset) override
    {
        return false;//not supported
    }

    void loadDefault(CubeMapAsset &asset, AssetManager& assetManager) override
    {
        asset.faces[CubeMapAsset::POS_X].loadFromFile(getFullAssetPath("/textures/px.png"));
        asset.faces[CubeMapAsset::NEG_X].loadFromFile(getFullAssetPath("/textures/nx.png"));
        asset.faces[CubeMapAsset::POS_Y].loadFromFile(getFullAssetPath("/textures/py.png"));
        asset.faces[CubeMapAsset::NEG_Y].loadFromFile(getFullAssetPath("/textures/ny.png"));
        asset.faces[CubeMapAsset::POS_Z].loadFromFile(getFullAssetPath("/textures/pz.png"));
        asset.faces[CubeMapAsset::NEG_Z].loadFromFile(getFullAssetPath("/textures/nz.png"));

        //use parameters set before loading from the first time
        asset.allocateOnGPU({});
    }
};
REGISTERCLASS(CubeMapList);