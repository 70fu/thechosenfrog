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
        loadDefault(assets[FramebufferIds::DEFAULT],assetManager);
        //...
    }

    bool loadAssetFromFile(const std::string &path, FramebufferAsset &asset) override
    {
        return false;//not supported
    }

    void loadDefault(FramebufferAsset &asset, AssetManager& assetManager) override
    {
        FramebufferParameters parameters;
        parameters.useTextureForDepthAndStencil = false;
        parameters.resizeOnResolutionChange = true;//resize to screen

        asset.allocateOnGPU(parameters);
    }
};
REGISTERCLASS(FramebufferList);