//
// Created by simon on 25.03.19.
//

#ifndef CGUE19_THECHOSENFROG_ASSETS_H
#define CGUE19_THECHOSENFROG_ASSETS_H

#include <soloud_wav.h>
#include <soloud_wavstream.h>

//forward asset type declerations
class Mesh;
class ShaderProgram;
class Material;
class Texture;
class Shader;
class CubeMap;
class BitmapFont;
class Framebuffer;
//asset aliases
using SoundAsset = SoLoud::Wav;
using MusicAsset = SoLoud::WavStream;
using MeshAsset = Mesh;
using ShaderAsset=Shader;
using ShaderProgramAsset = ShaderProgram;
using MaterialAsset = Material;
using TextureAsset = Texture;
using CubeMapAsset = CubeMap;
using BitmapFontAsset = BitmapFont;
using FramebufferAsset = Framebuffer;

using AssetId = unsigned int;

/**
 * Values are set such that bit masks are possible
 */
enum AssetType : unsigned short
{
    SOUND = 1,
    MUSIC = 2,
    SHADER=4,
    MESH=8,
    MATERIAL=16,
    TEXTURE=32,
    SHADER_PROGRAM=64,
    CUBE_MAP = 128,
    BITMAP_FONT=256,
    FRAMEBUFFER=512,

    ALL=65535
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
#include <string>
/**
 * @param assetPath, relative to asset directory, must start with a /, eg. /shaders/phong.vert
 * @return full path to asset
 */
#ifndef NDEBUG
static constexpr const char* PATH_TO_ASSETS_DIR = "../../Game/assets";//asset folder relative to cmake build directory
#else
static constexpr const char* PATH_TO_ASSETS_DIR = "./assets";//asset folder next to executable
#endif
inline std::string getFullAssetPath(const std::string& assetPath){
    return PATH_TO_ASSETS_DIR+assetPath;//from cmake build directory
}

#endif //CGUE19_THECHOSENFROG_ASSETS_H
