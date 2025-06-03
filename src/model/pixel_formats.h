#pragma once

using namespace std;
#include <cstdint>
#include <iostream>

namespace gimi {

enum class Chroma {
  rgb,
  rgba,
  bayer,
  gray,
  yuv_444,
  yuv_422,
  yuv_420,
  yuv_411,
  // multi_spectral,
};

enum class Interleave {
  planar,
  interleaved,
  // mixed,
  // row,
  // tile_component,
  // multi_y_pixel
};

enum class PixelType {
  uint8,
  uint10,
  uint12,
  uint14,
  uint16,
  int8,
  int16,
  float32,
  complex,
  mixed, // Each PixelType plane in RawImage different.
};

enum class Codec {
  raw,  // uncompressed
  hevc, // h265
  avc,  // h264
  vvc,  // h266
  av1,
  jpeg,
  j2k,
  htj2k,
};

// To String
string to_string(PixelType);
string to_string(Chroma);
string to_string(Interleave);
string to_string(Codec);

// Getters
uint32_t get_bit_depth(PixelType);
uint32_t get_component_count(Chroma);

} // namespace gimi