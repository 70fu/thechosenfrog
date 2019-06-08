//
// Created by simon on 22.03.19.
//

#include "AssetManager.h"
#include "../util/RuntimeCompileUtils.h"
#include "../RuntimeClasses.h"
#include "../Constants.h"
#include <RuntimeObjectSystem.h>
#include <iostream>

#define STRPOOL_IMPLEMENTATION
#define ASSETSYS_IMPLEMENTATION
#include "../cute_filewatch/assetsys.h"

#define CUTE_FILEWATCH_IMPLEMENTATION
#include "../cute_filewatch/cute_filewatch.h"

using namespace ImGuiAl;

/**
 * called whenever an asset changes
 * @param change
 * @param virtual_path
 * @param udata points to AssetManager
 */
void AssetManager::filewatchCallback(filewatch_update_t change, const char* virtual_path, void* udata)
{
    std::string change_string;
    switch (change)
    {
        case FILEWATCH_DIR_ADDED: change_string = "FILEWATCH_DIR_ADDED"; break;
        case FILEWATCH_DIR_REMOVED: change_string = "FILEWATCH_DIR_REMOVED"; break;
        case FILEWATCH_FILE_ADDED: change_string = "FILEWATCH_FILE_ADDED"; break;
        case FILEWATCH_FILE_REMOVED: change_string = "FILEWATCH_FILE_REMOVED"; break;
        case FILEWATCH_FILE_MODIFIED: change_string = "FILEWATCH_FILE_MODIFIED"; break;
    }

    ImGuiAl::Log::getInstance().Info("%s at %s",change_string.c_str(),virtual_path);

    if(change==FILEWATCH_FILE_MODIFIED)
        ((AssetManager*)udata)->reloadFileAsset(virtual_path);
}

void AssetManager::init(IRuntimeObjectSystem *ros)
{
    this->runtimeObjectSystem = ros;

    //register listener for runtime modified classes
    runtimeObjectSystem->GetObjectFactorySystem()->AddListener(this);

    //construct lists
    soundListId = RuntimeCompileUtils::constructObject(runtimeObjectSystem,RuntimeClassNames::SOUND_LIST,&soundList);
    musicListId = RuntimeCompileUtils::constructObject(runtimeObjectSystem,RuntimeClassNames::MUSIC_LIST,&musicList);
    meshListId = RuntimeCompileUtils::constructObject(runtimeObjectSystem,RuntimeClassNames::MESH_LIST,&meshList);
    materialListId = RuntimeCompileUtils::constructObject(runtimeObjectSystem,RuntimeClassNames::MATERIAL_LIST,&materialList);
    shaderListId = RuntimeCompileUtils::constructObject(runtimeObjectSystem,RuntimeClassNames::SHADER_LIST,&shaderList);
    shaderProgramListId = RuntimeCompileUtils::constructObject(runtimeObjectSystem,RuntimeClassNames::SHADER_PROGRAM_LIST,&shaderProgramList);
    textureListId = RuntimeCompileUtils::constructObject(runtimeObjectSystem,RuntimeClassNames::TEXTURE_LIST,&textureList);
    cubeMapListId = RuntimeCompileUtils::constructObject(runtimeObjectSystem,RuntimeClassNames::CUBE_MAP_LIST,&cubeMapList);
    bitmapFontListId = RuntimeCompileUtils::constructObject(runtimeObjectSystem,RuntimeClassNames::BITMAP_FONT_LIST,&bitmapFontList);
    framebufferListId = RuntimeCompileUtils::constructObject(runtimeObjectSystem,RuntimeClassNames::FRAMEBUFFER_LIST,&framebufferList);

    //load all assets
    loadAssets(AssetType::ALL);

#ifndef NDEBUG
    //init filewatch
    assetsys = assetsys_create(0);
    for(int i = 0;i<sizeof(filewatches)/sizeof(filewatch_t*);++i)
        filewatches[i] = filewatch_create(assetsys, 0);

    int i = 0;
    if(!filewatch_mount(filewatches[i], "../../Game/assets/settings", FILEWATCH_SETTINGS_PATH))
        Log::getInstance().Error("Failed to mount file watcher to settings directory");
    if(!filewatch_start_watching(filewatches[i], FILEWATCH_SETTINGS_PATH, AssetManager::filewatchCallback, this))
        Log::getInstance().Error("Failed to start watching settings directory");
    ++i;

    if(!filewatch_mount(filewatches[i], "../../Game/assets/shaders", FILEWATCH_SHADERS_PATH))
        Log::getInstance().Error("Failed to mount file watcher to shaders directory");
    if(!filewatch_start_watching(filewatches[i], FILEWATCH_SHADERS_PATH, AssetManager::filewatchCallback, this))
        Log::getInstance().Error("Failed to start watching shaders directory");
    ++i;

    if(!filewatch_mount(filewatches[i], "../../Game/assets/music", FILEWATCH_MUSIC_PATH))
        Log::getInstance().Error("Failed to mount file watcher to music directory");
    if(!filewatch_start_watching(filewatches[i], FILEWATCH_MUSIC_PATH, AssetManager::filewatchCallback, this))
        Log::getInstance().Error("Failed to start watching music directory");
    ++i;

    if(!filewatch_mount(filewatches[i], "../../Game/assets/sounds", FILEWATCH_SOUND_PATH))
        Log::getInstance().Error("Failed to mount file watcher to sound directory");
    if(!filewatch_start_watching(filewatches[i], FILEWATCH_SOUND_PATH, AssetManager::filewatchCallback, this))
        Log::getInstance().Error("Failed to start watching sound directory");
    ++i;

    if(!filewatch_mount(filewatches[i], "../../Game/assets/meshes", FILEWATCH_MESH_PATH))
        Log::getInstance().Error("Failed to mount file watcher to mesh directory");
    if(!filewatch_start_watching(filewatches[i], FILEWATCH_MESH_PATH, AssetManager::filewatchCallback, this))
        Log::getInstance().Error("Failed to start watching mesh directory");
    ++i;

    if(!filewatch_mount(filewatches[i], "../../Game/assets/textures", FILEWATCH_TEXTURE_PATH))
        Log::getInstance().Error("Failed to mount file watcher to texture directory");
    if(!filewatch_start_watching(filewatches[i], FILEWATCH_TEXTURE_PATH, AssetManager::filewatchCallback, this))
        Log::getInstance().Error("Failed to start watching textures directory");
    ++i;

    if(!filewatch_mount(filewatches[i], "../../Game/assets/fonts", FILEWATCH_FONT_PATH))
        Log::getInstance().Error("Failed to mount file watcher to font directory");
    if(!filewatch_start_watching(filewatches[i], FILEWATCH_FONT_PATH, AssetManager::filewatchCallback, this))
        Log::getInstance().Error("Failed to start watching font directory");
    ++i;
#endif
}


void AssetManager::update()
{
#ifndef NDEBUG
    //scan asset folder for changes if time is due
    fileWatchUpdateCounter-=FIXED_DELTA;
    if(fileWatchUpdateCounter<=0)
    {
        for(int i = 0;i<sizeof(filewatches)/sizeof(filewatch_t*);++i)
        {
            filewatch_update(filewatches[i]);
            filewatch_notify(filewatches[i]);
        }
        fileWatchUpdateCounter = FILEWATCH_UPDATE_INTERVAL;
    }
#endif
}

void AssetManager::cleanup()
{
    cleanupSounds();
    cleanupMusic();
    cleanupMeshes();
    cleanupMaterials();
    cleanupShaderPrograms();
    cleanupShaders();
    cleanupCubeMaps();
    cleanupTextures();
    cleanupBitmapFonts();
    cleanupFramebuffers();

#ifndef NDEBUG
    for(int i = 0;i<sizeof(filewatches)/sizeof(filewatch_t*);++i)
        filewatch_free(filewatches[i]);
    assetsys_destroy(assetsys);
#endif
}

SoundAsset* AssetManager::getSound(AssetId id) {
    if(id<SOUNDS_SIZE)
        return &sounds[id];
    else
        return &sounds[SoundIds::DEFAULT];
}

MusicAsset* AssetManager::getMusic(AssetId id) {
    if(id<MUSIC_SIZE)
        return &music[id];
    else
        return &music[MusicIds::DEFAULT];
}

MeshAsset *AssetManager::getMesh(AssetId id)
{
    if(id<MESH_SIZE)
        return &meshes[id];
    else
        return &meshes[MeshIds::DEFAULT];
}

MaterialAsset *AssetManager::getMaterial(AssetId id)
{
    if(id<MATERIAL_SIZE)
        return &materials[id];
    else
        return &materials[MaterialIds::DEFAULT];
}

ShaderAsset *AssetManager::getShader(AssetId id)
{
    if(id<SHADER_SIZE)
        return &shaders[id];
    else
        return &shaders[ShaderIds::DEFAULT];
}

ShaderProgramAsset *AssetManager::getShaderProgram(AssetId id)
{
    if(id<SHADER_PROGRAM_SIZE)
        return &shaderPrograms[id];
    else
        return &shaderPrograms[ShaderProgramIds::DEFAULT];
}

TextureAsset *AssetManager::getTexture(AssetId id)
{
    if(id<TEXTURE_SIZE)
        return &textures[id];
    else
        return &textures[TextureIds::DEFAULT];
}

CubeMapAsset *AssetManager::getCubeMap(AssetId id)
{
    if(id<CUBE_MAP_SIZE)
        return &cubeMaps[id];
    else
        return &cubeMaps[CubeMapIds::DEFAULT];
}

BitmapFontAsset *AssetManager::getBitmapFont(AssetId id)
{
    if(id<BITMAP_FONT_SIZE)
        return &bitmapFonts[id];
    else
        return &bitmapFonts[BitmapFontIds::DEFAULT];
}

FramebufferAsset *AssetManager::getFramebuffer(AssetId id)
{
    if(id<FRAMEBUFFER_SIZE)
        return &framebuffers[id];
    else
        return &framebuffers[FramebufferIds::DEFAULT];
}

void AssetManager::OnConstructorsAdded() {
    unsigned char assetBitmask = 0;
    if( soundList )
    {
        if(RuntimeCompileUtils::updateObject(runtimeObjectSystem,soundListId,&soundList))
            assetBitmask|=AssetType::SOUND;
    }
    if( musicList)
    {
        if(RuntimeCompileUtils::updateObject(runtimeObjectSystem,musicListId,&musicList))
            assetBitmask|=AssetType::MUSIC;
    }
    if(meshList)
    {
        if(RuntimeCompileUtils::updateObject(runtimeObjectSystem,meshListId,&meshList))
            assetBitmask|=AssetType::MESH;
    }
    if(materialList)
    {
        if(RuntimeCompileUtils::updateObject(runtimeObjectSystem,materialListId,&materialList))
            assetBitmask|=AssetType::MATERIAL;
    }
    if(shaderList)
    {
        if(RuntimeCompileUtils::updateObject(runtimeObjectSystem,shaderListId,&shaderList))
            assetBitmask|=AssetType::SHADER;
    }
    if(shaderProgramList)
    {
        if(RuntimeCompileUtils::updateObject(runtimeObjectSystem,shaderProgramListId,&shaderProgramList))
            assetBitmask|=AssetType::SHADER_PROGRAM;
    }
    if(textureList)
    {
        if(RuntimeCompileUtils::updateObject(runtimeObjectSystem,textureListId,&textureList))
            assetBitmask|=AssetType::TEXTURE;
    }
    if(cubeMapList)
    {
        if(RuntimeCompileUtils::updateObject(runtimeObjectSystem,cubeMapListId,&cubeMapList))
            assetBitmask|=AssetType::CUBE_MAP;
    }
    if(bitmapFontList)
    {
        if(RuntimeCompileUtils::updateObject(runtimeObjectSystem,bitmapFontListId,&bitmapFontList))
            assetBitmask|=AssetType::BITMAP_FONT;
    }
    if(framebufferList)
    {
        if(RuntimeCompileUtils::updateObject(runtimeObjectSystem,framebufferListId,&framebufferList))
            assetBitmask|=AssetType::FRAMEBUFFER;
    }

    //reload lists of asset types whose classes have changed
    loadAssets(assetBitmask);
}

void AssetManager::loadAssets(std::underlying_type<AssetType>::type assetBitmask)
{
    if(assetBitmask==0)
        return;

    //make a string path array that has the size of the biggest assets array, just in case
    std::pair<std::string,AssetIdentifier> paths[BIGGEST_SIZE];

    //load assets from asset lists
    if((assetBitmask&AssetType::SOUND)!=0)
        storeFilePaths(paths,soundList->loadAll(sounds,paths,SOUNDS_SIZE,*this));
    if((assetBitmask&AssetType::MUSIC)!=0)
        storeFilePaths(paths,musicList->loadAll(music,paths,MUSIC_SIZE,*this));
    if((assetBitmask&AssetType::MESH)!=0)
        storeFilePaths(paths,meshList->loadAll(meshes,paths,MESH_SIZE,*this));
    if((assetBitmask&AssetType::SHADER)!=0)
    {
        storeFilePaths(paths, shaderList->loadAll(shaders, paths, SHADER_SIZE, *this));
        //force reloading of shader programs as well
        assetBitmask|=AssetType::SHADER_PROGRAM;
    }
    if((assetBitmask&AssetType::SHADER_PROGRAM)!=0)
    {
        storeFilePaths(paths, shaderProgramList->loadAll(shaderPrograms, paths, SHADER_PROGRAM_SIZE, *this));

        //if a reload of materials has not been requested, then at least the uniform locations have to be updated
        if((assetBitmask&AssetType::MATERIAL)==0)
        {
            for(int j = 0;j<MATERIAL_SIZE && j<MaterialIds::MATERIAL_COUNT;++j)
            {
                MaterialAsset& material = materials[j];
                material.data.clearLocations();
                material.data.retrieveLocations(*material.shader);
            }
        }
    }
    if((assetBitmask&AssetType::TEXTURE)!=0)
        storeFilePaths(paths,textureList->loadAll(textures,paths,TEXTURE_SIZE,*this));
    if((assetBitmask&AssetType::MATERIAL)!=0)
        storeFilePaths(paths,materialList->loadAll(materials,paths,MATERIAL_SIZE,*this));
    if((assetBitmask&AssetType::CUBE_MAP)!=0)
        storeFilePaths(paths,cubeMapList->loadAll(cubeMaps,paths,CUBE_MAP_SIZE,*this));
    if((assetBitmask&AssetType::BITMAP_FONT)!=0)
        storeFilePaths(paths,bitmapFontList->loadAll(bitmapFonts,paths,BITMAP_FONT_SIZE,*this));
    if((assetBitmask&AssetType::FRAMEBUFFER)!=0)
        storeFilePaths(paths,framebufferList->loadAll(framebuffers,paths,FRAMEBUFFER,*this));
    //...
}

void AssetManager::reloadAssets()
{
    cleanupSounds();
    cleanupMusic();
    cleanupMeshes();
    cleanupMaterials();
    cleanupShaderPrograms();
    cleanupShaders();
    cleanupCubeMaps();
    cleanupTextures();
    cleanupBitmapFonts();
    cleanupFramebuffers();

    loadAssets(AssetType::ALL);
}

void AssetManager::reloadFileAsset(const std::string& path)
{
    //check if there is an asset with given path
    if(pathsToIds.count(path)==0)
    {
        Log::getInstance().Error("Could not find any asset at path %s",path.c_str());
        return;
    }

    AssetIdentifier id = pathsToIds[path];
    switch(id.assetType)
    {
        case AssetType::SOUND:
        {
            SoundAsset &sound = sounds[id.id];
            cleanupSound(sound);
            soundList->loadFromFile(path, sound, *this);
            break;
        }
        case AssetType::MUSIC:
        {
            MusicAsset& m = music[id.id];
            cleanupMusic(m);
            musicList->loadFromFile(path, m, *this);
            break;
        }
        case AssetType::MESH:
        {
            MeshAsset& mesh = meshes[id.id];
            cleanupMesh(mesh);
            meshList->loadFromFile(path, mesh, *this);
            break;
        }
        case AssetType::MATERIAL:
        {
            MaterialAsset& material = materials[id.id];
            cleanupMaterial(material);
            materialList->loadFromFile(path, material, *this);
            break;
        }
        case AssetType::SHADER:
        {
            Shader *shader = &shaders[id.id];
            cleanupShader(*shader);
            shaderList->loadFromFile(path, *shader, *this);

            //reload all shader programs using this shader
            for (int i = 0; i < SHADER_PROGRAM_SIZE && i < ShaderProgramIds::SHADER_PROGRAM_COUNT; ++i)
            {
                ShaderProgramAsset &shaderProgram = shaderPrograms[i];
                if (shaderProgram.hasShader(shader))
                {
                    //relink program
                    shaderProgram.relink();

                    //let materials using this program retrieve uniforms again
                    for(int j = 0;j<MATERIAL_SIZE && j<MaterialIds::MATERIAL_COUNT;++j)
                    {
                        MaterialAsset& material = materials[j];
                        if(material.shader==&shaderPrograms[i])
                        {
                            material.data.clearLocations();
                            material.data.retrieveLocations(*material.shader);
                        }
                    }
                }
            }
            break;
        }
        case AssetType::TEXTURE:
        {
            Texture& texture = textures[id.id];
            cleanupTexture(texture);
            textureList->loadFromFile(path, texture, *this);
        }
        case AssetType::CUBE_MAP:
        {
            CubeMapAsset& cubeMap = cubeMaps[id.id];
            cleanupCubeMap(cubeMap);
            cubeMapList->loadFromFile(path, cubeMap, *this);
            break;
        }
        case AssetType::BITMAP_FONT:
        {
            BitmapFont& font = bitmapFonts[id.id];
            cleanupBitmapFont(font);
            bitmapFontList->loadFromFile(path, font, *this);
            break;
        }
        default:
            Log::getInstance().Error("Asset type with id %d cannot be reloaded",id.assetType);
            break;
    }
}

void AssetManager::storeFilePaths(std::pair<std::string, AssetIdentifier> *paths, size_t size)
{
    for(int i = 0;i<size;++i)
        pathsToIds[paths[i].first] = paths[i].second;
}

void AssetManager::cleanupSounds()
{
    for(int i = 0;i<SOUNDS_SIZE && i<SoundIds::SOUND_COUNT;++i)
        cleanupSound(sounds[i]);
}

void AssetManager::cleanupSound(SoundAsset &soundAsset) {
    //There does not seem to be the need to clean anything up
}

void AssetManager::cleanupMusic() {
    for(int i = 0;i<MUSIC_SIZE && i<MusicIds::MUSIC_COUNT;++i)
        cleanupMusic(music[i]);
}

void AssetManager::cleanupMusic(MusicAsset &musicAsset) {
    //There does not seem to be the need to clean anything up
}

void AssetManager::cleanupMeshes()
{
    for(int i = 0;i<MESH_SIZE && i<MeshIds::MESH_COUNT;++i)
        cleanupMesh(meshes[i]);
}

void AssetManager::cleanupMesh(MeshAsset &meshAsset)
{
    meshAsset.cleanup();
}

void AssetManager::cleanupMaterials()
{
    for(int i = 0;i<MATERIAL_SIZE && i<MaterialIds::MATERIAL_COUNT;++i)
        cleanupMaterial(materials[i]);
}

void AssetManager::cleanupMaterial(MaterialAsset &materialAsset)
{
    //nothing to do here
}

void AssetManager::cleanupShaders()
{
    for(int i = 0;i<SHADER_SIZE && i<ShaderIds::SHADER_COUNT;++i)
        cleanupShader(shaders[i]);
}

void AssetManager::cleanupShader(ShaderAsset &shaderAsset)
{
    shaderAsset.cleanup();
}

void AssetManager::cleanupShaderPrograms()
{
    for(int i = 0;i<SHADER_PROGRAM_SIZE && i<ShaderProgramIds::SHADER_PROGRAM_COUNT;++i)
        cleanupShaderProgram(shaderPrograms[i]);
}

void AssetManager::cleanupShaderProgram(ShaderProgramAsset &shaderProgramAsset)
{
    shaderProgramAsset.cleanup();
}

void AssetManager::cleanupTextures()
{
    for(int i = 0;i<TEXTURE_SIZE && i<TextureIds::TEXTURE_COUNT;++i)
        cleanupTexture(textures[i]);
}

void AssetManager::cleanupTexture(TextureAsset &textureAsset)
{
    textureAsset.cleanup();
}

void AssetManager::cleanupCubeMaps()
{
    for (int i = 0; i < CUBE_MAP_SIZE && i < CubeMapIds::CUBE_MAP_COUNT; ++i)
        cleanupCubeMap(cubeMaps[i]);
}

void AssetManager::cleanupCubeMap(CubeMapAsset &cubeMap)
{
    cubeMap.cleanup();
}

void AssetManager::cleanupBitmapFonts()
{
    for(int i = 0;i<BITMAP_FONT_SIZE && i<BitmapFontIds::BITMAP_FONT_COUNT;++i)
        cleanupBitmapFont(bitmapFonts[i]);
}

void AssetManager::cleanupBitmapFont(BitmapFontAsset &bitmapFont)
{
    bitmapFont.cleanup();
}

void AssetManager::cleanupFramebuffers()
{
    for(int i = 0;i<FRAMEBUFFER_SIZE && i<FramebufferIds::FRAMEBUFFER_COUNT;++i)
        cleanupFramebuffer(framebuffers[i]);
}

void AssetManager::cleanupFramebuffer(FramebufferAsset& framebuffer)
{
    framebuffer.cleanup();
}
