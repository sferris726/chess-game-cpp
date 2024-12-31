file(GLOB_RECURSE UTILITIES_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

# Add the source file for Utilities
add_library(Utilities ${UTILITIES_SOURCES})

# Specify the include directory for this library (Utilities)
target_include_directories(Utilities PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
