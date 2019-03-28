//
// Created by simon on 22.03.19.
//

#include "AssetManager.h"
#include "../util/RuntimeCompileUtils.h"
#include "../RuntimeClasses.h"
#include <RuntimeObjectSystem.h>

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
}

void AssetManager::update()
{

}

void AssetManager::cleanup()
{
    cleanupSounds();
    cleanupMusic();
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
