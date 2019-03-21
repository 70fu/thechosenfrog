cmake_minimum_required(VERSION 3.12)
project(Game)

set(CMAKE_CXX_STANDARD 17)

add_executable(thechosenfrog src/main.cpp src/ConsoleGame.cpp src/ConsoleGame.h src/Updateable.cpp src/Updateable.h src/RuntimeTypeIds.h src/ConsoleUpdate.cpp src/StdioLogSystem.cpp src/StdioLogSystem.h
        src/util/INIReader.h
        src/imgui/imconfig.h src/imgui/imgui.cpp src/imgui/imgui.h src/imgui/imgui_draw.cpp src/imgui/imgui_demo.cpp src/imgui/imgui_internal.h src/imgui/imgui_widgets.cpp src/imgui/imstb_rectpack.h src/imgui/imstb_textedit.h src/imgui/imstb_truetype.h src/imgui/imgui_impl_glfw.cpp src/imgui/imgui_impl_glfw.h src/imgui/imgui_impl_opengl3.cpp src/imgui/imgui_impl_opengl3.h src/Game.cpp src/Game.h)

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

#copy assets to build dir
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/Game)