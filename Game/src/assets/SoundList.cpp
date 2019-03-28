//
// Created by simon on 22.03.19.
//

#ifndef CGUE19_THECHOSENFROG_SOUNDLIST_H
#define CGUE19_THECHOSENFROG_SOUNDLIST_H

#include <ObjectInterfacePerModule.h>
#include "../RuntimeClasses.h"
#include "AssetList.h"
#include <soloud_wav.h>
#include <RuntimeInclude.h>
#include "SoundIds.h"

class SoundList : public TInterface<RuntimeClassIds::SOUND_LIST,AssetList<SoLoud::Wav,AssetType::SOUND>>
{
protected:
    void loadAssets(SoLoud::Wav *assets, size_t size) const override
    {
        //TODO make loadFromFile calls, ...
    }

    bool loadAssetFromFile(const std::string &path, SoLoud::Wav &asset) const override
    {
        return asset.load(path.c_str())==SoLoud::SO_NO_ERROR;
    }

    void loadDefault(SoLoud::Wav &asset) const override {
        //TODO
    }
};
REGISTERCLASS(SoundList);



#endif //CGUE19_THECHOSENFROG_SOUNDLIST_H
