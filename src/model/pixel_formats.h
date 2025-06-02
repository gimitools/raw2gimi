#pragma once

using namespace std;
#include <string>

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
};

// ISO/IEC 23001-17 Table 4
enum class Interleave {
  planar,
  interleaved,
  // mixed,
  // row,
  // tile_component,
  // multi_y_pixel
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

enum class BitDepth {
  uint8,
  uint10,
  uint12,
  uint14,
  uint16,
  int8,
  int16,
  float32,
  complex,
  mixed, // Each BitDepth plane in RawImage different.
};

string to_string(BitDepth);
string to_string(Chroma);
string to_string(Interleave);
string to_string(Codec);

} // namespace gimi