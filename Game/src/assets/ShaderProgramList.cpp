
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

        //configure skybox shader program
        {
            ShaderProgramAsset& skybox = assets[ShaderProgramIds::SKYBOX];
            skybox.addShader(assetManager.getShader(ShaderIds::SKYBOX_VERT));
            skybox.addShader(assetManager.getShader(ShaderIds::SKYBOX_FRAG));
        }

        //configure unlit shader program
        {
            ShaderProgramAsset& unlit = assets[ShaderProgramIds::UNLIT];
            unlit.addShader(assetManager.getShader(ShaderIds::UNLIT_VERT));
            unlit.addShader(assetManager.getShader(ShaderIds::UNLIT_FRAG));
        }

        //configure font shader program
        {
            ShaderProgramAsset& font = assets[ShaderProgramIds::FONT];
            font.addShader(assetManager.getShader(ShaderIds::FONT_VERT));
            font.addShader(assetManager.getShader(ShaderIds::FONT_GEOM));
            font.addShader(assetManager.getShader(ShaderIds::FONT_FRAG));
        }

        //configure parabola shader program
        {
            ShaderProgramAsset& font = assets[ShaderProgramIds::PARABOLA];
            font.addShader(assetManager.getShader(ShaderIds::PARABOLA_VERT));
            font.addShader(assetManager.getShader(ShaderIds::PARABOLA_GEOM));
            font.addShader(assetManager.getShader(ShaderIds::PARABOLA_FRAG));
        }

        //configure fbo shader
        {
            ShaderProgramAsset& fbo = assets[ShaderProgramIds::FBO];
            fbo.addShader(assetManager.getShader(ShaderIds::FBO_VERT));
            fbo.addShader(assetManager.getShader(ShaderIds::FBO_FRAG));
        }

        //link all shader programs, skip default program
        for(size_t i = 1;i<ShaderProgramIds::SHADER_PROGRAM_COUNT && i<size;++i)
        {
            if(!assets[i].link())
            {
                ImGuiAl::Log::getInstance().Error("Using default shader program due to linking error in shader program %d",i);
                loadDefault(assets[i],assetManager);
            }
        }
        //...
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
};
REGISTERCLASS(ShaderProgramList);
