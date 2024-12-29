cmake_minimum_required(VERSION 3.10)
project(ChessGame)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BINARY_DIR "${CMAKE_SOURCE_DIR}/build")

# Add subdirectories for the source files
add_subdirectory(src/Game)
add_subdirectory(src/Board)
add_subdirectory(src/Pieces)

# Add source files
add_executable(ChessGame src/main.cpp)

target_link_libraries(ChessGame Game Board)
