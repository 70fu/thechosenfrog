//
// Created by simon on 22.03.19.
//

#ifndef CGUE19_THECHOSENFROG_MUSICLIST_H
#define CGUE19_THECHOSENFROG_MUSICLIST_H

#include <soloud_wavstream.h>
#include "../RuntimeClasses.h"
#include "AssetList.h"
#include <ObjectInterfacePerModule.h>
#include "MusicIds.h"

class MusicList : public TInterface<RuntimeClassIds::SOUND_LIST,AssetList<SoLoud::WavStream,AssetType::MUSIC>>
{
protected:
    void loadAssets(SoLoud::WavStream *assets, size_t size) const override
    {
        //TODO make loadFromFile calls, ...
    }

    bool loadAssetFromFile(const std::string &path, SoLoud::WavStream &asset) const override
    {
        return asset.load(path.c_str())==SoLoud::SO_NO_ERROR;
    }

    void loadDefault(SoLoud::WavStream &asset) const override
    {
        //TODO
    }
};
REGISTERCLASS(MusicList);



#endif //CGUE19_THECHOSENFROG_MUSICLIST_H
