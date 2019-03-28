//
// Created by simon on 22.03.19.
//

#ifndef CGUE19_THECHOSENFROG_ASSETLIST_H
#define CGUE19_THECHOSENFROG_ASSETLIST_H

#include <IObject.h>
#include <string>
#include "Assets.h"

/**
 * Base class to simplify writing of asset loader using inheritance
 * A base class should use the protected loadAssetsFromFileHelper method to load an asset from file (which calls the virtual loadAssetFromFile function)
 * Can load all assets at once filling an array and returns all used file paths using an array
 * @tparam A
 */
template<class A,AssetType AT>
class AssetList : public IObject
{
private:
    int fileLoadCounter = 0;
    std::pair<std::string,AssetIdentifier>* paths = nullptr;
protected:
    /**
     * loads all assets, and stores them in given array
     * should use loadFromFile and never loadAssetsFromFileHelper directly
     * @param assets
     * @param size
     */
    virtual void loadAssets(A assets[], size_t size) const=0;

    /**
     * Loads the asset file into given asset object
     * should only be used from loadAssets
     * @param path !=nullptr
     * @param asset
     * @param path
     */
    void loadAssetsFromFileHelper(const std::string& path, AssetId id, A& asset)
    {
        if(paths!=nullptr)
            paths[fileLoadCounter++] = {path,{AT,id}};

        loadFromFile(path,asset);
    }

    /**
     * Loads the asset file into given asset object
     * @param path !=nullptr
     * @param asset
     * @param path
     * @return true if asset could successfully be loaded from file
     */
    virtual bool loadAssetFromFile(const std::string& path, A& asset) const=0;
public:
    /**
     * Loads all assets into given array.
     * Every asset loaded from a file is returned via the paths array
     * @param sounds to fill
     * @param paths, path of every asset loaded from a file, may not be filled completely, see return values
     * @param size of sounds and paths
     * @return how many assets were loaded from files (file paths have been set into paths array until this value (exclusive))
     */
    int loadAll(A assets[], std::pair<std::string,AssetIdentifier> paths[],size_t size)
    {
        //TODO what if the size is smaller than the sound count

        fileLoadCounter = 0;
        this->paths = paths;

        //load assets
        loadAssets(assets,size);

        this->paths = nullptr;
        return fileLoadCounter;
    }

    /**
     * loads the default asset, this is called when asset could not be loaded from file
     * @param asset
     */
    virtual void loadDefault(A& asset) const=0;

    /**
     * Loads the asset file into given asset object
     * should only be used from loadAssets
     * @param path !=nullptr
     * @param asset
     * @param path
     */
    void loadFromFile(const std::string& path, A& asset)
    {
        if(!loadAssetFromFile(path,asset))
        {
            //TODO log message

            //reset asset to default
            loadDefault(asset);
        }
    }
};

#endif //CGUE19_THECHOSENFROG_ASSETLIST_H