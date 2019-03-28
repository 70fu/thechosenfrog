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
#include "AssetList.h"
//#include "SoundList.cpp"
//#include "MusicList.cpp"

#ifndef NDEBUG
#define EXTRA_ASSET_SPACE 0
#else
#define EXTRA_ASSET_SPACE 16
#endif

//forward declarations
struct IRuntimeObjectSystem;
class SoundList;
class MusicList;

//calculate array sizes at compile time
static constexpr size_t SOUNDS_SIZE = SoundIds::SOUND_COUNT+EXTRA_ASSET_SPACE;
static constexpr size_t MUSIC_SIZE = MusicIds::MUSIC_COUNT+EXTRA_ASSET_SPACE;
static constexpr size_t BIGGEST_SIZE = std::max<size_t>({SOUNDS_SIZE,MUSIC_SIZE});

class AssetManager : public IObjectFactoryListener
{
private:
    //asset arrays
    SoundAsset sounds[SOUNDS_SIZE];
    MusicAsset music[MUSIC_SIZE];

    //asset paths & file watching
    std::unordered_map<std::string,AssetIdentifier> pathsToIds;

    IRuntimeObjectSystem* runtimeObjectSystem;

    //assets lists
    AssetList<SoundAsset,AssetType::SOUND>* soundList;
    ObjectId soundListId;
    AssetList<MusicAsset,AssetType::MUSIC>* musicList;
    ObjectId musicListId;
public:
    //loads all assets
    void init(IRuntimeObjectSystem* runtimeObjectSystem);
    void update();
    //cleans up all assets
    void cleanup();

    void OnConstructorsAdded() override;

    SoundAsset* getSound(AssetId id);
    MusicAsset* getMusic(AssetId id);

private:
    void loadAssets(unsigned char assetBitmask);
    void reloadFileAsset(const std::string& path);
    void reloadAssets();

    void storeFilePaths(std::pair<std::string,AssetIdentifier> paths[],int size);

    //asset loading and cleanup functions
    void cleanupSounds();
    void cleanupSound(SoundAsset& soundAsset);

    void cleanupMusic();
    void cleanupMusic(MusicAsset& musicAsset);
};


#endif //CGUE19_THECHOSENFROG_ASSETMANAGER_H