# cgue19-thechosenfrog
Computergraphik Übung SS 19 - OpenGL & C++

## Libraries
- [GLFW](https://www.glfw.org/) - Windowing Toolkit
- [GLM](https://github.com/g-truc/glm) - Math
- [Dear ImGui](https://github.com/ocornut/imgui) - Debug GUI
- [SoLoud](https://github.com/jarikomppa/soloud) - Audio
- [RuntimeCompiledCpp](https://github.com/RuntimeCompiledCPlusPlus/RuntimeCompiledCPlusPlus)
- [Assimp 5.0.0 RC1](https://github.com/assimp/assimp) - Model Loader


## which files to upload
.sln-Solution
external, content folder

## don't upload
.sdf - Intellisens Database
.suo, .user - User settings
bin/build

## Links

- [GLFW Input Key](https://www.glfw.org/docs/latest/input_guide.html#input_key) - How to set up Key & Mouse Input

## How to add a new Asset:
1. Go to <Asset>Ids.h and add a new id to the enum
1. Go to <Asset>List.h and write asset at the same position in the assets array as the new id (just use the id for index)
 1. If the asset is loaded from a file, use loadAssetFromFileHelper()
