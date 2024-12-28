#!/bin/bash

# Format C++ and CMake files
format_files() {
  find . -type f \( -name "*.cpp" -o -name "*.h" -o -name "CMakeLists.txt" \) ! -path "./build/*" | while read file; do
    # Check if it's a CMakeLists.txt file and sort its includes
    if [[ "$file" == *CMakeLists.txt ]]; then
      echo "Formatting CMake file: $file"
      # Here, you can run cmake-format (or clang-format if preferred)
      cmake-format -i "$file" || clang-format -i "$file"
    else
      # For cpp and h files, sort the includes alphabetically
      echo "Formatting and sorting includes in: $file"
      
      # Sort the includes alphabetically and save to a temporary file
      # Assuming that includes are at the top of the file, before code starts
      awk 'BEGIN {includes=1} 
           /^#include/ {print $0; next} 
           {includes=0} 
           !includes' "$file" | sort > "$file.sorted"
      
      # Now prepend the sorted includes to the rest of the file
      awk 'BEGIN {includes=1} 
           /^#include/ {print $0; next} 
           {includes=0} 
           !includes' "$file" > "$file.sorted"
      
      mv "$file.sorted" "$file"
      
      # Run clang-format to format the file
      clang-format -i "$file"
    fi
  done
}

# Run the function
format_files
