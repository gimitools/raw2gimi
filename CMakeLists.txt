cmake_minimum_required(VERSION 3.10)
project(raw2gimi)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g") # Turn off optimizations for debugging

# Add libheif submodule
set(WITH_UNCOMPRESSED_CODEC ON CACHE BOOL "Enable Uncompressed codec in libheif")
set(LIBHEIF_DIR submodules/libheif)
add_subdirectory(${LIBHEIF_DIR})  # Builds libheif from source

# Automatically find all .cpp files recursively
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

# Include libheif headers
target_include_directories(${PROJECT_NAME} BEFORE PRIVATE
  ${LIBHEIF_DIR}/libheif/api/
)

# Link against libheif
target_link_libraries(${PROJECT_NAME} PRIVATE heif)

# === Add LibRaw ===
find_path(LIBRAW_INCLUDE_DIR libraw/libraw.h)
find_library(LIBRAW_LIBRARY NAMES raw)

if(NOT LIBRAW_INCLUDE_DIR OR NOT LIBRAW_LIBRARY)
  message(FATAL_ERROR "LibRaw not found")
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${LIBRAW_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBRAW_LIBRARY})
