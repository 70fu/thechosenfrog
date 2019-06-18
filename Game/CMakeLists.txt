cmake_minimum_required(VERSION 3.11)
project(Game)

set(CMAKE_CXX_STANDARD 17)

add_executable(thechosenfrog src/main.cpp src/StdioLogSystem.cpp src/StdioLogSystem.h
		src/IEventManager.h 
		src/EventManager.cpp
        src/util/INIReader.h
		src/cute_filewatch/assetsys.h
		src/cute_filewatch/cute_filewatch.h
		src/Game.cpp
		src/Game.h
		src/components/TransformComponent.cpp
		src/components/TransformComponent.h
		src/Constants.h
		src/assets/AssetManager.cpp
		src/assets/AssetManager.h
		src/assets/SoundList.cpp
		src/RuntimeClasses.h
		src/assets/AssetList.h
		src/assets/MusicList.cpp
		src/util/RuntimeCompileUtils.h
		src/assets/SoundIds.h 
		src/assets/MusicIds.h
		src/assets/Assets.h
		src/RuntimeSoLoud.h
		src/logger/imguial_log.h
		src/logger/imguial_log.cpp
		src/IDebugGUI.h src/IDebugGUI.cpp
		src/ImGuiDebugGUI.cpp 
		src/assets/Mesh.cpp src/assets/Mesh.h
        src/assets/Surface.cpp src/assets/Surface.h
		src/assets/MeshList.cpp
		src/assets/MeshIds.h
		src/assets/ShaderProgram.cpp src/assets/ShaderProgram.h
		src/assets/Material.cpp src/assets/Material.h
		src/assets/Texture.cpp src/assets/Texture.h
		src/assets/MaterialList.cpp
		src/assets/MaterialIds.h
		src/assets/ShaderList.cpp
		src/assets/ShaderProgramIds.h
		src/assets/ShaderIds.h
		src/assets/Shader.cpp src/assets/Shader.h
		src/assets/ShaderProgramList.cpp
		src/DebugWindowIds.h
		src/components/Components.h src/components/Components.cpp
		src/components/MeshComponent.cpp src/components/MeshComponent.h
		src/components/Component.cpp src/components/Component.h
		src/components/Entity.h

	#-----libs
		libs/imgui/imconfig.h
		libs/imgui/imgui.cpp
		libs/imgui/imgui.h
		libs/imgui/imgui_draw.cpp
		libs/imgui/imgui_demo.cpp
		libs/imgui/imgui_internal.h
		libs/imgui/imgui_widgets.cpp
		libs/imgui/imstb_rectpack.h
		libs/imgui/imstb_textedit.h
		libs/imgui/imstb_truetype.h
		libs/imgui/imgui_impl_glfw.cpp
		libs/imgui/imgui_impl_glfw.h
		libs/imgui/imgui_impl_opengl3.cpp
		libs/imgui/imgui_impl_opengl3.h

		libs/stb_image/stb_image.h

		libs/stb_truetype/stb_truetype.h

		#------end libs
		src/IGameRenderer.h
		src/GameRenderer.cpp
		src/IGameUpdater.h
		src/GameUpdater.cpp
		src/components/MaterialComponent.cpp src/components/MaterialComponent.h
		src/components/CameraComponent.cpp src/components/CameraComponent.h
		src/util/CameraUtil.cpp src/util/CameraUtil.h
		src/scenes/IScene.h
		src/scenes/MainScene.cpp
		src/components/CameraControllerComponent.cpp src/components/CameraControllerComponent.h
		src/assets/ImageData.cpp src/assets/ImageData.h
		src/assets/TextureList.cpp src/assets/TextureIds.h
		src/util/Color.h
		src/Physics.cpp src/Physics.h
		src/components/PhysicsComponent.cpp src/components/PhysicsComponent.h
		src/assets/CubeMap.cpp src/assets/CubeMap.h
		src/assets/CubeMapIds.h
		src/assets/CubeMapList.cpp
		src/components/CharControllerComponent.cpp src/components/CharControllerComponent.h
		src/components/PlayerComponent.cpp src/components/PlayerComponent.h
		src/components/PlatformComponent.cpp src/components/PlatformComponent.h
		src/util/GLMPXConversion.cpp src/util/GLMPXConversion.h src/assets/BitmapFont.cpp src/assets/BitmapFont.h src/assets/BitmapFontList.cpp src/assets/BitmapFontIds.h src/components/TextComponent.cpp src/components/TextComponent.h src/AppSettings.cpp src/AppSettings.h src/assets/Framebuffer.cpp src/assets/Framebuffer.h src/assets/FramebufferList.cpp src/assets/FramebufferIds.h src/components/CloudComponent.cpp src/components/CloudComponent.h src/CloudPlatforms.cpp src/CloudPlatforms.h src/CharControllerConfiguration.cpp src/CharControllerConfiguration.h src/CloudGen.h)

#imgui
target_include_directories(thechosenfrog PRIVATE libs/imgui)

#stb_image
target_include_directories(thechosenfrog PRIVATE libs/stb_image)

#stb_truetype
target_include_directories(thechosenfrog PRIVATE libs/stb_truetype)

#runtime compiled cpp
target_include_directories(thechosenfrog PRIVATE ${CMAKE_SOURCE_DIR}/RuntimeCompiledCPlusPlus/RuntimeCompiler)
target_include_directories(thechosenfrog PRIVATE ${CMAKE_SOURCE_DIR}/RuntimeCompiledCPlusPlus/RuntimeObjectSystem)
target_link_libraries(thechosenfrog RuntimeObjectSystem)

#glm
add_definitions(-DGLM_ENABLE_EXPERIMENTAL)
target_include_directories(thechosenfrog PRIVATE libs/glm)


#GLAD
set(GLAD_DIR "libs/glad")
add_library("glad" "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")
target_include_directories(thechosenfrog PRIVATE "${GLAD_DIR}/include")
target_link_libraries(thechosenfrog "glad" "${CMAKE_DL_LIBS}")

#GLFW
target_include_directories(thechosenfrog PRIVATE ${CMAKE_SOURCE_DIR}/glfw/include)
target_link_libraries(thechosenfrog glfw ${GLFW_LIBRARIES})

#soloud
target_include_directories(thechosenfrog PRIVATE ${CMAKE_SOURCE_DIR}/soloud/include)
target_link_libraries(thechosenfrog soloud)

#assimp
target_include_directories(thechosenfrog PRIVATE ${ASSIMP_INCLUDE_DIR})
target_link_libraries(thechosenfrog assimp)

#Physx
#include headers
target_include_directories(thechosenfrog PRIVATE ${CMAKE_SOURCE_DIR}/physx/include)

#link to correct libraries
set(PHYSX_BUILD_CONFIG checked)#todo choose release on release build

if(WIN32)
	set(PHYSX_PLATFORM windows)
elseif(UNIX)
	set(PHYSX_PLATFORM linux)
endif()

set(PHYSX_LIB_PATH ${CMAKE_SOURCE_DIR}/physx/${PHYSX_PLATFORM}/${PHYSX_BUILD_CONFIG}/)

find_library(PHYSX_LIB NAMES PhysX PhysX_32 PhysX_64 PhysX_static_32 PhysX_static_64 PATHS ${PHYSX_LIB_PATH})
find_library(PHYSX_COMMON_LIB NAMES PhysXCommon PhysXCommon_32 PhysXCommon_64 PhysXCommon_static_32 PhysXCommon_static_64 PATHS ${PHYSX_LIB_PATH})
find_library(PHYSX_EXTENSION_LIB NAMES PhysXExtensions PhysXExtensions_32 PhysXExtensions_64 PhysXExtensions_static_32 PhysXExtensions_static_64 PATHS ${PHYSX_LIB_PATH})
find_library(PHYSX_FOUNDATION_LIB NAMES PhysXFoundation PhysXFoundation_32 PhysXFoundation_64 PhysXFoundation_static_32 PhysXFoundation_static_64 PATHS ${PHYSX_LIB_PATH})
find_library(PHYSX_PVD_LIB NAMES PhysXPvdSDK PhysXPvdSDK_32 PhysXPvdSDK_64 PhysXPvdSDK_static_32 PhysXPvdSDK_static_64 PATHS ${PHYSX_LIB_PATH})
find_library(PHYSX_COOKING_LIB NAMES PhysXCooking PhysXCooking_32 PhysXCooking_64 PhysXCooking_static_32 PhysXCooking_static_64 PATHS ${PHYSX_LIB_PATH})
find_library(PHYSX_CHARACTER_LIB NAMES PhysXCharacterKinematic PhysXCharacterKinematic_32 PhysXCharacterKinematic_64 PhysXCharacterKinematic_static_32 PhysXCharacterKinematic_static_64 PATHS ${PHYSX_LIB_PATH})
find_library(PHYSX_VEHICLE_LIB NAMES PhysXVehicle PhysXVehicle_32 PhysXVehicle_64 PhysXVehicle_static_32 PhysXVehicle_static_64 PATHS ${PHYSX_LIB_PATH})


if(CMAKE_CXX_COMPILER_ID MATCHES GNU OR CMAKE_CXX_COMPILER_ID MATCHES CLANG)
	target_link_libraries(thechosenfrog -Wl,--start-group ${PHYSX_LIB} ${PHYSX_COMMON_LIB} ${PHYSX_EXTENSION_LIB} ${PHYSX_FOUNDATION_LIB} ${PHYSX_PVD_LIB} ${PHYSX_COOKING_LIB} ${PHYSX_CHARACTER_LIB} ${PHYSX_VEHICLE_LIB} -Wl,--end-group)
else()
	set(PHYSX_LIBS ${PHYSX_FOUNDATION_LIB} ${PHYSX_COMMON_LIB} ${PHYSX_LIB} ${PHYSX_EXTENSION_LIB} ${PHYSX_PVD_LIB} ${PHYSX_COOKING_LIB} ${PHYSX_CHARACTER_LIB} ${PHYSX_VEHICLE_LIB})
	target_link_libraries(thechosenfrog ${PHYSX_LIBS})
endif()

#physx needs to have either NDEBUG or (exclusively) _DEBUG defined
if (CMAKE_BUILD_TYPE MATCHES Debug)
	add_definitions(-D_DEBUG)
endif()
