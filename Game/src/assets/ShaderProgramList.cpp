
#include "AssetList.h"
#include "../RuntimeClasses.h"
#include "ShaderProgramIds.h"
#include "ShaderIds.h"
#include <ObjectInterfacePerModule.h>
#include "AssetManager.h"

class ShaderProgramList : public TInterface<RuntimeClassIds::SHADER_PROGRAM_LIST,AssetList<ShaderProgramAsset,AssetType::SHADER_PROGRAM>>
{
protected:
    void loadAssets(ShaderProgramAsset *assets, size_t size, AssetManager& assetManager) override
    {
		loadDefault(assets[ShaderProgramIds::DEFAULT], assetManager);
		 //...
		loadSkybox(assets[ShaderProgramIds::SKYBOX],assetManager);
       
    }

    bool loadAssetFromFile(const std::string &path, ShaderProgramAsset &asset) override
    {
        return false;//not supported currently
    }

    void loadDefault(ShaderProgramAsset &asset, AssetManager& assetManager) override
    {
        asset.addShader(assetManager.getShader(ShaderIds::DEFAULT_VERT));
        asset.addShader(assetManager.getShader(ShaderIds::DEFAULT_FRAG));
        asset.link();
    }

	void loadSkybox(ShaderProgramAsset &asset, AssetManager& assetManager)
	{
		asset.addShader(assetManager.getShader(ShaderIds::SKYBOX_VERT));
		asset.addShader(assetManager.getShader(ShaderIds::SKYBOX_FRAG));
		asset.link();
	}
};
REGISTERCLASS(ShaderProgramList);
