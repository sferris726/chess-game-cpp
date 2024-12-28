#!/bin/bash

# Directory to start from (current directory by default)
START_DIR="${1:-.}"

# Find all C++ files (including .cpp, .h, .hpp, .cc, .hh) and CMake files (CMakeLists.txt, *.cmake)
CXX_FILES=$(find "$START_DIR" -type f \( -iname "*.cpp" -o -iname "*.h" -o -iname "*.hpp" -o -iname "*.cc" -o -iname "*.hh" \) ! -path "./build/*")
CMAKE_FILES=$(find "$START_DIR" -type f \( -iname "CMakeLists.txt" -o -iname "*.cmake" \) ! -path "./build/*")

# Check if clang-format and cmake-format are installed
if ! command -v clang-format &> /dev/null; then
    echo "clang-format not found. Please install it first."
    exit 1
fi

if ! command -v cmake-format &> /dev/null; then
    echo "cmake-format not found. Please install it first."
    exit 1
fi

# Format all C++ files with clang-format
for FILE in $CXX_FILES; do
    echo "Formatting C++ file: $FILE"
    clang-format -i "$FILE"
done

# Format all CMake files with cmake-format
for FILE in $CMAKE_FILES; do
    echo "Formatting CMake file: $FILE"
    cmake-format -i "$FILE"
done

echo "Formatting complete."
