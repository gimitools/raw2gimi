cmake_minimum_required(VERSION 3.10)
project(raw2gimi)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g") # Turn off optimizations for debugging

# Add libheif submodule
set(HEIF_ENABLE_EXPERIMENTAL_FEATURES ON CACHE BOOL "Enable experimental features in libheif")
set(WITH_UNCOMPRESSED_CODEC ON CACHE BOOL "Enable Uncompressed codec in libheif")
set(LIBHEIF_DIR submodules/libheif)

### Find OpenJPEG ### (TODO: remove hardcoded path)
set(OpenJPEG_DIR "/usr/local/lib/cmake/openjpeg-2.5" CACHE PATH "Path to OpenJPEGConfig.cmake")
set(WITH_OpenJPEG_ENCODER ON CACHE BOOL "Enable OpenJPEG encoder support")
set(WITH_OpenJPEG_DECODER ON CACHE BOOL "Enable OpenJPEG decoder support")
set(ENABLE_PLUGIN_LOADING OFF CACHE BOOL "")
set(WITH_OpenJPEG_ENCODER_PLUGIN OFF CACHE BOOL "")
set(WITH_OpenJPEG_DECODER_PLUGIN OFF CACHE BOOL "")
set(CMAKE_PREFIX_PATH "/usr/local" CACHE STRING "Prefix for finding OpenJPEG")
message(STATUS "[raw2gimi] OpenJPEG_DIR = ${OpenJPEG_DIR}")
message(STATUS "[raw2gimi] CMAKE_PREFIX_PATH = ${CMAKE_PREFIX_PATH}")
### Find OpenJPEG ### (TODO: remove hardcoded path)

add_subdirectory(${LIBHEIF_DIR})  # Builds libheif from source

# Automatically find all .cpp files recursively
file(GLOB_RECURSE SOURCES "src/*.cpp")

# ================= Redland =================
find_package(PkgConfig REQUIRED)
pkg_check_modules(REDLAND REQUIRED redland)
# ================= Redland =================

# ================= Executable =================
add_executable(${PROJECT_NAME} ${SOURCES})
# ================= Executable =================

target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src/model/
  ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

# Include libheif headers
target_include_directories(${PROJECT_NAME} PRIVATE
  ${LIBHEIF_DIR}/libheif/api/
)

# Link against libheif
target_link_libraries(${PROJECT_NAME} PRIVATE heif)


# ================= Redland =================
target_include_directories(${PROJECT_NAME} PRIVATE ${REDLAND_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${REDLAND_LIBRARIES})
# ================= Redland =================


# === Add LibRaw ===
find_path(LIBRAW_INCLUDE_DIR libraw/libraw.h)
find_library(LIBRAW_LIBRARY NAMES raw)

if(NOT LIBRAW_INCLUDE_DIR OR NOT LIBRAW_LIBRARY)
  message(FATAL_ERROR "LibRaw not found")
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${LIBRAW_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBRAW_LIBRARY})
