cmake_minimum_required(VERSION 3.12)
project(Game)

set(CMAKE_CXX_STANDARD 17)

add_executable(thechosenfrog src/main.cpp src/StdioLogSystem.cpp src/StdioLogSystem.h
		src/IEventManager.h 
		src/EventManager.cpp
        src/util/INIReader.h
		src/cute_filewatch/assetsys.h src/cute_filewatch/cute_filewatch.h
        libs/imgui/imconfig.h libs/imgui/imgui.cpp libs/imgui/imgui.h libs/imgui/imgui_draw.cpp libs/imgui/imgui_demo.cpp libs/imgui/imgui_internal.h libs/imgui/imgui_widgets.cpp libs/imgui/imstb_rectpack.h libs/imgui/imstb_textedit.h libs/imgui/imstb_truetype.h libs/imgui/imgui_impl_glfw.cpp libs/imgui/imgui_impl_glfw.h libs/imgui/imgui_impl_opengl3.cpp libs/imgui/imgui_impl_opengl3.h src/Game.cpp src/Game.h src/Constants.h src/assets/AssetManager.cpp src/assets/AssetManager.h src/assets/SoundList.cpp src/RuntimeClasses.h src/assets/AssetList.h src/assets/MusicList.cpp src/util/RuntimeCompileUtils.h src/assets/SoundIds.h src/assets/MusicIds.h src/assets/Assets.h src/RuntimeSoLoud.h
		src/log/imguial_log.h src/log/imguial_log.cpp src/IDebugGUI.h src/ImGuiDebugGUI.cpp src/DebugWindowIds.h src/IDebugGUI.cpp)

#imgui
target_include_directories(thechosenfrog PRIVATE libs/imgui)

#runtime compiled cpp
target_include_directories(thechosenfrog PRIVATE ${CMAKE_SOURCE_DIR}/RuntimeCompiledCPlusPlus/RuntimeCompiler)
target_include_directories(thechosenfrog PRIVATE ${CMAKE_SOURCE_DIR}/RuntimeCompiledCPlusPlus/RuntimeObjectSystem)
target_link_libraries(thechosenfrog RuntimeObjectSystem)

#glm
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