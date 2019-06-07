#include "AssetList.h"
#include "../RuntimeClasses.h"
#include <ObjectInterfacePerModule.h>
#include "Assets.h"
#include "Framebuffer.h"
#include "AssetManager.h"

class FramebufferList : public TInterface<RuntimeClassIds::FRAMEBUFFER_LIST,AssetList<FramebufferAsset ,AssetType::FRAMEBUFFER>>
{
protected:
    void loadAssets(FramebufferAsset *assets, size_t size, AssetManager& assetManager) override
    {
        //loadDefault(assets[CubeMapIds::DEFAULT],assetManager);
        //TODO create framebuffers
        //...
    }

    bool loadAssetFromFile(const std::string &path, FramebufferAsset &asset) override
    {
        return false;//not supported
    }

    void loadDefault(FramebufferAsset &asset, AssetManager& assetManager) override
    {
        //TODO
    }
};
REGISTERCLASS(FramebufferList);