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

//forward declare string
#include <iosfwd>
/**
 * @param assetPath, relative to asset directory, must start with a /, eg. /shaders/phong.vert
 * @return full path to asset
 */
#ifndef NDEBUG
static constexpr const char* PATH_TO_ASSETS_DIR = "../../Game/assets";//asset folder relative to cmake build directory
#else
static constexpr const char* PATH_TO_ASSETS_DIR = "./assets";//asset folder next to executable
#endif
std::string getFullAssetPath(const std::string& assetPath);

#endif //CGUE19_THECHOSENFROG_ASSETS_H
