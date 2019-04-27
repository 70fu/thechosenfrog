#include "AssetList.h"
#include "../RuntimeClasses.h"
#include <ObjectInterfacePerModule.h>
#include "Assets.h"
#include "Texture.h"
#include "AssetManager.h"

class TextureList : public TInterface<RuntimeClassIds::TEXTURE_LIST,AssetList<TextureAsset ,AssetType::TEXTURE>>
{
protected:
    void loadAssets(TextureAsset *assets, size_t size, AssetManager& assetManager) override
    {
        loadDefault(assets[TextureIds::DEFAULT],assetManager);
        //you can set parameters on the texture before loading it
        //...
    }

    bool loadAssetFromFile(const std::string &path, TextureAsset &asset) override
    {
        asset.data.loadFromFile(path);
        //use parameters set before loading from the first time
        asset.allocateOnGPU();
    }

    void loadDefault(TextureAsset &asset, AssetManager& assetManager) override
    {
        //make 2x2 checkerboard texture
        ImageData image(32,32,3);

        Color red(255,0,0,255);
        Color blue(0,0,255,255);
        for(unsigned int x = 0;x<16;++x)
        {
            for (unsigned int y = 0; y < 16; ++y)
                image.setPixel(x, y, red);
        }
        for(unsigned int x = 16;x<32;++x)
        {
            for (unsigned int y = 0; y < 16; ++y)
                image.setPixel(x, y, blue);
        }
        for(unsigned int x = 0;x<16;++x)
        {
            for (unsigned int y = 16; y < 32; ++y)
                image.setPixel(x, y, blue);
        }
        for(unsigned int x = 16;x<32;++x)
        {
            for (unsigned int y = 16; y < 32; ++y)
                image.setPixel(x, y, red);
        }

        asset.data = std::move(image);
        asset.allocateOnGPU({GL_TEXTURE_2D,false,GL_NEAREST,GL_NEAREST,GL_REPEAT,GL_REPEAT});
    }
};
REGISTERCLASS(TextureList);
