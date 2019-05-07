
#include "AssetList.h"
#include "../RuntimeClasses.h"
#include <ObjectInterfacePerModule.h>
#include <sstream>
#include "Assets.h"
#include "BitmapFont.h"
#include "AssetManager.h"

class BitmapFontList : public TInterface<RuntimeClassIds::BITMAP_FONT_LIST,AssetList<BitmapFontAsset ,AssetType::BITMAP_FONT>>
{
protected:
    void loadAssets(BitmapFontAsset *assets, size_t size, AssetManager& assetManager) override
    {
        assets[BitmapFontIds::DEFAULT].parameters = {};
        assets[BitmapFontIds::DEFAULT].parameters.bitmapParams = {GL_TEXTURE_2D,true,GL_NEAREST_MIPMAP_NEAREST,GL_NEAREST,GL_REPEAT,GL_REPEAT};
        loadAssetsFromFileHelper("/fonts/Emo.ttf",BitmapFontIds::DEFAULT,assets[BitmapFontIds::DEFAULT],assetManager);
        //you can configure parameters before loading the ttf font
        //...
    }

    bool loadAssetFromFile(const std::string &path, BitmapFontAsset &asset) override
    {
        //read whole file into byte array from https://stackoverflow.com/a/36658802
        std::ifstream iStream(path,std::ifstream::binary | std::ifstream::in);

        if(!iStream.is_open())
        {
            ImGuiAl::Log::getInstance().Error("Failed to find bitmap font %s",path.c_str());
            return false;
        }

        //get length of file
        iStream.seekg(0, std::ifstream::end);
        size_t length = iStream.tellg();
        iStream.seekg(0, std::ifstream::beg);

        //allocate buffer on dynamic memory
        char* buffer = new char[length];

        //read file
        if(!iStream.read(&buffer[0], length))
        {
            ImGuiAl::Log::getInstance().Error("Unable to load bytes of ttf file %s",path.c_str());
            return false;
        }

        //generate bitmap
        asset.generateBitmapFromTTF(reinterpret_cast<unsigned char*>(buffer));

        return true;
    }

    void loadDefault(BitmapFontAsset &asset, AssetManager& assetManager) override
    {
        asset.parameters = {};
        asset.parameters.bitmapParams = {GL_TEXTURE_2D,true,GL_NEAREST_MIPMAP_NEAREST,GL_NEAREST,GL_REPEAT,GL_REPEAT};
        loadAssetFromFile(getFullAssetPath("/fonts/Emo.ttf"),asset);
    }
};
REGISTERCLASS(BitmapFontList);