file(GLOB_RECURSE PIECES_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

# Create a library from all .cpp files found
add_library(Pieces ${PIECES_SOURCES})

# Specify the include directory for this library (Pieces)
target_include_directories(Pieces PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
                                         ${CMAKE_CURRENT_SOURCE_DIR}/interfaces)
