file(GLOB_RECURSE BOARD_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# Add the source file for Board
add_library(Board ${BOARD_SOURCES})

# Specify the include directory for this library (Board)
target_include_directories(Board PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})