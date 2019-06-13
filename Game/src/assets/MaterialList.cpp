#include "AssetList.h"
#include "../RuntimeClasses.h"
#include <ObjectInterfacePerModule.h>
#include "Assets.h"
#include "Material.h"
#include "MaterialIds.h"
#include "AssetManager.h"

class MaterialList : public TInterface<RuntimeClassIds::MATERIAL_LIST,AssetList<MaterialAsset ,AssetType::MATERIAL>>
{
protected:
    void loadAssets(MaterialAsset *assets, size_t size, AssetManager& assetManager) override
    {
        loadDefault(assets[MaterialIds::DEFAULT],assetManager);
        //region unlit material
        {
            MaterialAsset& unlit = assets[MaterialIds::UNLIT];
            unlit.shader = assetManager.getShaderProgram(ShaderProgramIds::UNLIT);
            unlit.data.setTexture("diffuseTex",assetManager.getTexture(TextureIds::WHITE_SQUARE));
            unlit.data.setVec2("texRepeat",{1,1});
            unlit.data.setColor("colorMultiply",{255,255,255,255});

            unlit.data.retrieveLocations(*unlit.shader);
        }
        //endregion
        //region fbo post processing material
        {
            MaterialAsset& fboPost = assets[MaterialIds::FBO_POST];
            fboPost.shader = assetManager.getShaderProgram(ShaderProgramIds::FBO_POST);
            TextureAsset* whiteSquare = assetManager.getTexture(TextureIds::WHITE_SQUARE);
            //fboPost.data.setTexture("normalTexture",whiteSquare);
            fboPost.data.setInt("normalSampleSize",1);
            fboPost.data.setFloat("normalThreshold",0.5f);
            fboPost.data.setVec4("normalOutlineColor",{0,0,0,1});
            //fboPost.data.setTexture("depthTexture",whiteSquare);
            fboPost.data.setInt("depthSampleSize",3);
            fboPost.data.setFloat("depthThreshold",1);
            fboPost.data.setVec4("depthOutlineColor",{0,0,0,1});
            //fboPost.data.setFloat("brightness",1);

            fboPost.data.retrieveLocations(*fboPost.shader);
        }
        //endregion
    }

    bool loadAssetFromFile(const std::string &path, MaterialAsset &asset) override
    {
        return false;//currently not supported
    }

    void loadDefault(MaterialAsset &asset, AssetManager& assetManager) override
    {
        asset.data.clearProps();
        asset.shader = assetManager.getShaderProgram(ShaderProgramIds::DEFAULT);
    }
};
REGISTERCLASS(MaterialList);
