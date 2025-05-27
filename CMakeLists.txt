cmake_minimum_required(VERSION 3.10)
project(raw_to_gimi)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g") # Turn off optimizations for debugging

# Add libheif submodule
set(LIBHEIF_DIR submodules/libheif)
add_subdirectory(${LIBHEIF_DIR})  # Builds libheif from source

# Add your executable
add_executable(${PROJECT_NAME} src/main.cpp)

# Include libheif headers
target_include_directories(${PROJECT_NAME} BEFORE PRIVATE
  ${LIBHEIF_DIR}/libheif/api/
)

# Link against libheif target
target_link_libraries(${PROJECT_NAME} PRIVATE heif)
