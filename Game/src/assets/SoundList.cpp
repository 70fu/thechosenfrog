//
// Created by simon on 22.03.19.
//

#ifndef CGUE19_THECHOSENFROG_SOUNDLIST_H
#define CGUE19_THECHOSENFROG_SOUNDLIST_H

#include "../RuntimeSoLoud.h"
#include <ObjectInterfacePerModule.h>
#include "../RuntimeClasses.h"
#include "AssetList.h"
#include "SoundIds.h"

class SoundList : public TInterface<RuntimeClassIds::SOUND_LIST,AssetList<SoundAsset,AssetType::SOUND>>
{
protected:
    void loadAssets(SoundAsset *assets, size_t size, AssetManager& assetManager) override
    {
        loadAssetsFromFileHelper("/sounds/error.wav",SoundIds::DEFAULT,assets[SoundIds::DEFAULT],assetManager);
    }

    bool loadAssetFromFile(const std::string &path,SoundAsset &asset) override
    {
        return asset.load(path.c_str())==SoLoud::SO_NO_ERROR;
    }

    void loadDefault(SoundAsset &asset, AssetManager& assetManager) override
    {
        asset.load(getFullAssetPath("/sounds/error.wav").c_str());
    }
};
REGISTERCLASS(SoundList);



#endif //CGUE19_THECHOSENFROG_SOUNDLIST_H
