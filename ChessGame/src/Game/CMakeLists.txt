file(GLOB_RECURSE GAME_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

# Create a library from all .cpp files found
add_library(Game ${GAME_SOURCES})

# Specify the include directory for this library (Game)
target_include_directories(
  Game
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/interfaces
         ${CMAKE_CURRENT_SOURCE_DIR}/../Board
         ${CMAKE_CURRENT_SOURCE_DIR}/../Pieces)

target_link_libraries(Game PRIVATE Board Pieces)
