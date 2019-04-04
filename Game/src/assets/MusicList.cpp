//
// Created by simon on 22.03.19.
//

#ifndef CGUE19_THECHOSENFROG_MUSICLIST_H
#define CGUE19_THECHOSENFROG_MUSICLIST_H

#include "../RuntimeSoLoud.h"
#include "../RuntimeClasses.h"
#include "AssetList.h"
#include <ObjectInterfacePerModule.h>
#include "MusicIds.h"

class MusicList : public TInterface<RuntimeClassIds::MUSIC_LIST,AssetList<MusicAsset ,AssetType::MUSIC>>
{
protected:
    void loadAssets(MusicAsset *assets, size_t size) override
    {
        loadAssetsFromFileHelper("/music/error.ogg",MusicIds::DEFAULT,assets[MusicIds::DEFAULT]);
        //...
    }

    bool loadAssetFromFile(const std::string &path, MusicAsset &asset) override
    {
        return asset.load(path.c_str())==SoLoud::SO_NO_ERROR;
    }

    void loadDefault(MusicAsset &asset) override
    {
        asset.load(getFullAssetPath("/music/error.ogg").c_str());
    }
};
REGISTERCLASS(MusicList);



#endif //CGUE19_THECHOSENFROG_MUSICLIST_H
