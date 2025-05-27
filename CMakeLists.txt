cmake_minimum_required(VERSION 3.10)
project(dng_to_gimi)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g") # Turn off optimizations for debugging


# Find LibRaw
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBRAW REQUIRED libraw)


# Set ${HOME_DIR} to be ~
execute_process(COMMAND echo $ENV{HOME}
                OUTPUT_VARIABLE HOME_DIR
                OUTPUT_STRIP_TRAILING_WHITESPACE)


###################### LIBHEIF ######################
# Libheif minimum version required is 1.19.0.
# You'll likely neet to build from source to get a suitable version.
set(LIBHEIF_DIR ${HOME_DIR}/repos/libheif) # Root Directory for libheif

# set(LIBHEIF_BUILD_DIR ${LIBHEIF_DIR}/build)
set(LIBHEIF_BUILD_DIR ${LIBHEIF_DIR}/out/build/develop)

# Sets #define for experimental features
# target_compile_definitions(${LIBHEIF_BUILD_DIR} PRIVATE HEIF_ENABLE_EXPERIMENTAL_FEATURES)
###################### LIBHEIF ######################


include_directories(
  ${LIBRAW_INCLUDE_DIRS}
  ${LIBHEIF_DIR}/libheif/api
  ${LIBHEIF_BUILD_DIR} # contains libheif/heif_version.h
)

link_directories(
  ${LIBRAW_LIBRARY_DIRS}
  ${LIBHEIF_LIBRARY_DIRS}
)

add_definitions(
  ${LIBRAW_CFLAGS_OTHER}
  ${LIBHEIF_CFLAGS_OTHER}
)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME}
  ${LIBRAW_LIBRARIES}
  ${LIBHEIF_BUILD_DIR}/libheif/libheif.so
)