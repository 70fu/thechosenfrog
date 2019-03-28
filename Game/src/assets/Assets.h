//
// Created by simon on 25.03.19.
//

#ifndef CGUE19_THECHOSENFROG_ASSETS_H
#define CGUE19_THECHOSENFROG_ASSETS_H

#include <soloud_wav.h>
#include <soloud_wavstream.h>

//asset aliases
using SoundAsset = SoLoud::Wav;
using MusicAsset = SoLoud::WavStream;

using AssetId = unsigned int;

/**
 * Values are set such that bit masks are possible
 */
enum AssetType : unsigned char
{
    SOUND = 1,
    MUSIC = 2,
    //SHADER=4
    //MODEL=8,
    //TEXTURE=16,
    //FONT=32,

    ALL=255
};

/**
 * Uniquely identifies an asset in an asset manager
 */
struct AssetIdentifier
{
public:
    AssetType assetType;
    AssetId id;
};

#endif //CGUE19_THECHOSENFROG_ASSETS_H
