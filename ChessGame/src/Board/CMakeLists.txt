# Add the source file for Board
add_library(Board Board.cpp)

# Specify the include directory for this library (Board)
target_include_directories(Board PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})