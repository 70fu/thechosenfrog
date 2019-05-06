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
        makeWhiteSquareTexture(assets[TextureIds::WHITE_SQUARE],assetManager);
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

private:
    inline void makeWhiteSquareTexture(TextureAsset &asset, AssetManager& assetManager)
    {
        //create 8x8 white texture
        static constexpr unsigned int SQUARE_SIZE = 8;
        ImageData image(SQUARE_SIZE,SQUARE_SIZE,3);
        Color white(255,255,255,255);
        for(int x = 0;x<SQUARE_SIZE;++x)
        {
            for(int y = 0;y<SQUARE_SIZE;++y)
            {
                image.setPixel(x,y,white);
            }
        }

        asset.data = std::move(image);
        asset.allocateOnGPU({GL_TEXTURE_2D,false,GL_NEAREST,GL_NEAREST,GL_REPEAT,GL_REPEAT});
    }
};
REGISTERCLASS(TextureList);
