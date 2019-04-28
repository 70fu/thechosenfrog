//
// Created by simon on 22.03.19.
//

#ifndef CGUE19_THECHOSENFROG_ASSETMANAGER_H
#define CGUE19_THECHOSENFROG_ASSETMANAGER_H

#include <string>
#include <unordered_map>
#include <IObjectFactorySystem.h>
#include "SoundIds.h"
#include "MusicIds.h"
#include <algorithm>
#include "Assets.h"
#include "Mesh.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include "AssetList.h"
#include "../cute_filewatch/cute_filewatch.h"
#include "MeshIds.h"
#include "MaterialIds.h"
#include "Material.h"
#include "ShaderIds.h"
#include "ShaderProgramIds.h"
#include "TextureIds.h"
#include "CubeMap.h"
#include "CubeMapIds.h"

//#include "SoundList.cpp"
//#include "MusicList.cpp"

#ifndef NDEBUG
#define EXTRA_ASSET_SPACE 16
#else
#define EXTRA_ASSET_SPACE 0
#endif

//forward declarations
struct IRuntimeObjectSystem;

//calculate array sizes at compile time
static constexpr size_t SOUNDS_SIZE = SoundIds::SOUND_COUNT+EXTRA_ASSET_SPACE;
static constexpr size_t MUSIC_SIZE = MusicIds::MUSIC_COUNT+EXTRA_ASSET_SPACE;
static constexpr size_t MESH_SIZE = MeshIds::MESH_COUNT+EXTRA_ASSET_SPACE;
static constexpr size_t MATERIAL_SIZE = MaterialIds::MATERIAL_COUNT+EXTRA_ASSET_SPACE;
static constexpr size_t SHADER_SIZE = ShaderIds::SHADER_COUNT+EXTRA_ASSET_SPACE;
static constexpr size_t SHADER_PROGRAM_SIZE = ShaderProgramIds::SHADER_PROGRAM_COUNT+EXTRA_ASSET_SPACE;
static constexpr size_t TEXTURE_SIZE = TextureIds::TEXTURE_COUNT+EXTRA_ASSET_SPACE;
static constexpr size_t CUBE_MAP_SIZE = CubeMapIds::CUBE_MAP_COUNT+EXTRA_ASSET_SPACE;
static constexpr size_t BIGGEST_SIZE = std::max<size_t>({SOUNDS_SIZE,MUSIC_SIZE,MESH_SIZE,MATERIAL_SIZE,SHADER_SIZE,SHADER_PROGRAM_SIZE,TEXTURE_SIZE,CUBE_MAP_SIZE});

/**
 * Class that loads and manages assets, assets are hotswappable
 */
class AssetManager : public IObjectFactoryListener
{
private:
    //asset arrays
    SoundAsset sounds[SOUNDS_SIZE];
    MusicAsset music[MUSIC_SIZE];
    MeshAsset meshes[MESH_SIZE];
    MaterialAsset materials[MATERIAL_SIZE];
    ShaderAsset shaders[SHADER_SIZE];
    ShaderProgramAsset shaderPrograms[SHADER_PROGRAM_SIZE];
    TextureAsset textures[TEXTURE_SIZE];
    CubeMapAsset cubeMaps[CUBE_MAP_SIZE];

    //asset paths (relative from asset directory) & file watching
    std::unordered_map<std::string,AssetIdentifier> pathsToIds;

    //Asset manager is not responsible for this
    IRuntimeObjectSystem* runtimeObjectSystem;

    //assets lists
    AssetList<SoundAsset,AssetType::SOUND>* soundList;
    ObjectId soundListId;
    AssetList<MusicAsset,AssetType::MUSIC>* musicList;
    ObjectId musicListId;
    AssetList<MeshAsset ,AssetType::MESH>* meshList;
    ObjectId meshListId;
    AssetList<MaterialAsset ,AssetType::MATERIAL>* materialList;
    ObjectId materialListId;
    AssetList<ShaderAsset ,AssetType::SHADER>* shaderList;
    ObjectId shaderListId;
    AssetList<ShaderProgramAsset ,AssetType::SHADER_PROGRAM>* shaderProgramList;
    ObjectId shaderProgramListId;
    AssetList<TextureAsset ,AssetType::TEXTURE>* textureList;
    ObjectId textureListId;
    AssetList<CubeMapAsset ,AssetType::CUBE_MAP>* cubeMapList;
    ObjectId cubeMapListId;

#ifndef NDEBUG
    //file watcher
    static constexpr double FILEWATCH_UPDATE_INTERVAL = 1;
    double fileWatchUpdateCounter = FILEWATCH_UPDATE_INTERVAL;
    assetsys_t* assetsys;
    filewatch_t* filewatches[6];

    //virtual file watcher path
    static constexpr const char* FILEWATCH_SETTINGS_PATH = "/settings";
    static constexpr const char* FILEWATCH_SHADERS_PATH = "/shaders";
    static constexpr const char* FILEWATCH_MUSIC_PATH = "/music";
    static constexpr const char* FILEWATCH_SOUND_PATH = "/sounds";
    static constexpr const char* FILEWATCH_MESH_PATH = "/meshes";
    static constexpr const char* FILEWATCH_TEXTURE_PATH = "/textures";
    //...
#endif
public:
    /**
     * Initializes the asset manager and loads all assets
     * @param runtimeObjectSystem, !=nullptr
     */
    void init(IRuntimeObjectSystem* runtimeObjectSystem);
    /**
     * Checks if assets need to be reloaded and reloads them
     */
    void update();
    /**
     * cleans up all assets (eg. delete things on graphics card,...)
     */
    void cleanup();

    //called if Asset lists have been changed and recompiled
    void OnConstructorsAdded() override;

    /**
     * @param id
     * @return pointer to sound asset with given id, or default sound if there no sound with given id, never returns nullptr
     */
    SoundAsset* getSound(AssetId id);
    /**
     * @param id
     * @return pointer to music asset with given id, or default music if there no music with given id, never returns nullptr
     */
    MusicAsset* getMusic(AssetId id);
    /**
     * @param id
     * @return pointer to mesh asset with given id, or default mesh if there no mesh with given id, never returns nullptr
     */
    MeshAsset* getMesh(AssetId id);
    /**
     * @param id
     * @return pointer to material asset with given id, or default material if there no material with given id, never returns nullptr
     */
    MaterialAsset* getMaterial(AssetId id);
    /**
     * @param id
     * @return pointer to shader asset with given id, or default shader if there is no shader with given id, never returns nullptr
     */
    ShaderAsset* getShader(AssetId id);

    /**
     * @param id
     * @return pointer to shader program asset with given id, or default shader program if there is no shader program with given id, never returns nullptr
     */
    ShaderProgramAsset* getShaderProgram(AssetId id);

    /**
     * @param id
     * @return pointer to texture asset with given id, or default texture if there is no texture with given id, never returns nullptr
     */
    TextureAsset* getTexture(AssetId id);

    /**
     * @param id
     * @return pointer to cube map asset with given id, or default cube map if there is no cube map with given id, never returns nullptr
     */
    CubeMapAsset * getCubeMap(AssetId id);
private:
    /**
     * loads assets from asset lists, with given asset bitmask only certain asset types can be loaded selectively
     * @param assetBitmask
     */
    void loadAssets(std::underlying_type<AssetType>::type assetBitmask);
    void reloadFileAsset(const std::string& path);
    /**
     * Reloads all assets
     */
    void reloadAssets();

    /**
     * Stores values in given array into the path map, helper method for loading assets
     * @param paths, !=nullptr, length must be at least size
     * @param size
     */
    void storeFilePaths(std::pair<std::string,AssetIdentifier> paths[],size_t size);

    //asset loading and cleanup functions
    void cleanupSounds();
    void cleanupSound(SoundAsset& soundAsset);

    void cleanupMusic();
    void cleanupMusic(MusicAsset& musicAsset);

    void cleanupMeshes();
    void cleanupMesh(MeshAsset& meshAsset);

    void cleanupMaterials();
    void cleanupMaterial(MaterialAsset& materialAsset);

    void cleanupShaders();
    void cleanupShader(ShaderAsset& shaderAsset);

    void cleanupShaderPrograms();
    void cleanupShaderProgram(ShaderProgramAsset& shaderProgramAsset);

    void cleanupTextures();
    void cleanupTexture(Texture& textures);

    void cleanupCubeMaps();
    void cleanupCubeMap(CubeMapAsset& cubeMap);

    static void filewatchCallback(filewatch_update_t change, const char* virtual_path, void* udata);
};


#endif //CGUE19_THECHOSENFROG_ASSETMANAGER_H
