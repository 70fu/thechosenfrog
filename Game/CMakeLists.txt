cmake_minimum_required(VERSION 3.12)
project(Game)

set(CMAKE_CXX_STANDARD 17)

add_executable(thechosenfrog src/main.cpp src/ConsoleGame.cpp src/ConsoleGame.h src/Updateable.cpp src/Updateable.h src/RuntimeTypeIds.h src/ConsoleUpdate.cpp src/StdioLogSystem.cpp src/StdioLogSystem.h
        src/imgui/imconfig.h src/imgui/imgui.cpp src/imgui/imgui.h src/imgui/imgui_draw.cpp src/imgui/imgui_demo.cpp src/imgui/imgui_internal.h src/imgui/imgui_widgets.cpp src/imgui/imstb_rectpack.h src/imgui/imstb_textedit.h src/imgui/imstb_truetype.h)
include_directories(${CMAKE_SOURCE_DIR}/RuntimeCompiledCPlusPlus/RuntimeCompiler)
include_directories(${CMAKE_SOURCE_DIR}/RuntimeCompiledCPlusPlus/RuntimeObjectSystem)
target_link_libraries(thechosenfrog RuntimeObjectSystem)
