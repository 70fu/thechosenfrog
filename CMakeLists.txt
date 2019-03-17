cmake_minimum_required(VERSION 3.12)
project(cgue19_thechosenfrog)

add_subdirectory(glfw)
add_subdirectory(RuntimeCompiledCPlusPlus)
add_subdirectory(soloud)

#assimp
set(ASSIMP_NO_EXPORT ON)#disable exporters
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF)#disable tools
set(ASSIMP_BUILD_TESTS OFF)#disable tests
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT FALSE)#disable all formats to selectively enable needed ones
#enable needed model formats
set(ASSIMP_BUILD_OBJ_IMPORTER ON)
add_subdirectory(assimp)

add_subdirectory(Game)