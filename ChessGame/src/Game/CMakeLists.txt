file(GLOB_RECURSE GAME_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# Create a library from all .cpp files found
add_library(Game ${GAME_SOURCES})

# Specify the include directory for this library (Game)
target_include_directories(Game PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})