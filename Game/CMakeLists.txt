cmake_minimum_required(VERSION 3.12)
project(Game)

set(CMAKE_CXX_STANDARD 17)

add_executable(thechosenfrog main.cpp ConsoleGame.cpp ConsoleGame.h Updateable.cpp Updateable.h RuntimeTypeIds.h ConsoleUpdate.cpp StdioLogSystem.cpp StdioLogSystem.h)
include_directories(${CMAKE_SOURCE_DIR}/RuntimeCompiledCPlusPlus/RuntimeCompiler)
include_directories(${CMAKE_SOURCE_DIR}/RuntimeCompiledCPlusPlus/RuntimeObjectSystem)
target_link_libraries(thechosenfrog RuntimeObjectSystem)
