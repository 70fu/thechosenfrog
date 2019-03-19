cmake_minimum_required(VERSION 3.12)
project(cgue19_thechosenfrog)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs" FORCE)#disable exporters
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs" FORCE)#disable exporters

add_subdirectory(glfw)
add_subdirectory(RuntimeCompiledCPlusPlus)
add_subdirectory(soloud)

#assimp
set(ASSIMP_NO_EXPORT ON CACHE BOOL "Disable Assimp's export functionality." FORCE)#disable exporters
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "If the supplementary tools for Assimp are built in addition to the library." FORCE)#disable tools
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "If the test suite for Assimp is built in addition to the library." FORCE)#disable tests
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT FALSE CACHE BOOL "default value of all ASSIMP_BUILD_XXX_IMPORTER value" FORCE)#disable all formats to selectively enable needed ones
#enable needed model formats
set(ASSIMP_BUILD_OBJ_IMPORTER ON)
add_subdirectory(assimp)

add_subdirectory(Game)