#include "AssetList.h"
#include "../RuntimeClasses.h"
#include "Shader.h"
#include <ObjectInterfacePerModule.h>
#include <fstream>
#include <sstream>
#include "ShaderIds.h"

class ShaderList : public TInterface<RuntimeClassIds::SHADER_LIST,AssetList<ShaderAsset,AssetType::SHADER>>
{
protected:
    void loadAssets(ShaderAsset *assets, size_t size, AssetManager& assetManager) override
    {
        loadDefault(assets[ShaderIds::DEFAULT],assetManager);
        loadAssetsFromFileHelper("/shaders/default.vert",ShaderIds::DEFAULT_VERT,assets[ShaderIds::DEFAULT_VERT],assetManager);
        loadAssetsFromFileHelper("/shaders/default.frag",ShaderIds::DEFAULT_FRAG,assets[ShaderIds::DEFAULT_FRAG],assetManager);
        loadAssetsFromFileHelper("/shaders/unlit.vert",ShaderIds::UNLIT_VERT,assets[ShaderIds::UNLIT_VERT],assetManager);
        loadAssetsFromFileHelper("/shaders/unlit.frag",ShaderIds::UNLIT_FRAG,assets[ShaderIds::UNLIT_FRAG],assetManager);
        //...
    }

    bool loadAssetFromFile(const std::string &path, ShaderAsset &asset) override
    {
        //load whole text from file, taken from https://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
        std::ifstream iStream(path);
        std::stringstream sStream;
        sStream << iStream.rdbuf();
        //for (std::string line; std::getline(iStream, line);)
        //	sStream << line;

        asset.sourceCode = sStream.str();

        //get shader type from file ending
        size_t lastDotI = path.find_last_of('.');
        if(lastDotI==std::string::npos)
        {
            ImGuiAl::Log::getInstance().Error("Shader %s does not have a valid file ending (.frag or .vert), shader type could not be deduced",path.c_str());
            return false;
        }

        //TODO OH NO, hard coded valueasset.compile();s, plz remove
        //update trello doc if this is changed
        std::string fileEnding = path.substr(lastDotI+1);
        if(fileEnding=="vert")
            asset.type=GL_VERTEX_SHADER;
        else if(fileEnding=="frag")
            asset.type=GL_FRAGMENT_SHADER;
        else
        {
            ImGuiAl::Log::getInstance().Error("File Ending %s is not a valid file ending (.frag or .vert), shader type could not be deduced",fileEnding.c_str());
            return false;
        }

        //compile
        bool compileResult = asset.compile();
        if(!compileResult)
            asset.cleanup();
        return compileResult;
    }

    void loadDefault(ShaderAsset &asset, AssetManager& assetManager) override
    {
        //actually pretty stupid to have a single default shader, since there are many shader types but here is a default vertex shader
        asset.type = GL_VERTEX_SHADER;
        asset.sourceCode = "void main() { gl_Position = gl_Vertex; }";
        asset.compile();
    }
};
REGISTERCLASS(ShaderList);
