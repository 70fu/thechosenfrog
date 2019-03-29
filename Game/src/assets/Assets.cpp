//
// Created by simon on 29.03.19.
//



#include "Assets.h"
#include <string>

std::string getFullAssetPath(const std::string& assetPath) {
    return PATH_TO_ASSETS_DIR+assetPath;//from cmake build directory
}

