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

    //TODO log file change
    //std::cout<<change_string <<" at " <<virtual_path<<std::endl;

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

    //load all assets
    loadAssets(AssetType::ALL);

#ifndef NDEBUG
    //init filewatch
    assetsys = assetsys_create(0);
    filewatch = filewatch_create(assetsys, 0);

    if(!filewatch_mount(filewatch, "../../Game/assets/settings", FILEWATCH_SETTINGS_PATH))
        ;//TODO log
    if(!filewatch_start_watching(filewatch, FILEWATCH_SETTINGS_PATH, AssetManager::filewatchCallback, this))
        ;//TODO log
    if(!filewatch_mount(filewatch, "../../Game/assets/shaders", FILEWATCH_SHADERS_PATH))
        ;//TODO log
    if(!filewatch_start_watching(filewatch, FILEWATCH_SHADERS_PATH, AssetManager::filewatchCallback, this))
        ;//TODO log
#endif
}


void AssetManager::update()
{
#ifndef NDEBUG
    //scan asset folder for changes if time is due
    fileWatchUpdateCounter-=FIXED_DELTA;
    if(fileWatchUpdateCounter<=0)
    {
        filewatch_update(filewatch);
        filewatch_notify(filewatch);

        fileWatchUpdateCounter = FILEWATCH_UPDATE_INTERVAL;
    }
#endif
}

void AssetManager::cleanup()
{
    cleanupSounds();
    cleanupMusic();

#ifndef NDEBUG
    filewatch_free(filewatch);
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

    //reload lists of asset types whose classes have changed
    loadAssets(assetBitmask);
}

void AssetManager::loadAssets(unsigned char assetBitmask)
{
    if(assetBitmask==0)
        return;

    //make a string path array that has the size of the biggest assets array, just in case
    std::pair<std::string,AssetIdentifier> paths[BIGGEST_SIZE];

    //load assets from asset lists
    if((assetBitmask&AssetType::SOUND)!=0)
        storeFilePaths(paths,soundList->loadAll(sounds,paths,SOUNDS_SIZE));
    if((assetBitmask&AssetType::MUSIC)!=0)
        storeFilePaths(paths,musicList->loadAll(music,paths,MUSIC_SIZE));
    //...
}

void AssetManager::reloadAssets()
{
    cleanup();
    loadAssets(AssetType::ALL);
}

void AssetManager::reloadFileAsset(const std::string& path)
{
    //check if there is an asset with given path
    if(pathsToIds.count(path)==0)
    {
        //TODO log, could not find asset at path
        return;
    }

    AssetIdentifier id = pathsToIds[path];
    switch(id.assetType)
    {
        case AssetType::SOUND:
            soundList->loadFromFile(path,sounds[id.id]);
            break;
        case AssetType::MUSIC:
            musicList->loadFromFile(path,music[id.id]);
            break;
        default:
            //TODO log: unknown asset type
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
    for(SoundAsset& sound : sounds)
        cleanupSound(sound);
}

void AssetManager::cleanupSound(SoundAsset &soundAsset) {
    //There does not seem to be the need to clean anything up
}

void AssetManager::cleanupMusic() {
    for(MusicAsset & m : music)
        cleanupMusic(m);
}

void AssetManager::cleanupMusic(MusicAsset &musicAsset) {
    //There does not seem to be the need to clean anything up
}
