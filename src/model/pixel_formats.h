#pragma once

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
  UINT8,
  UINT10,
  UINT12,
  UINT14,
  UINT16,
  INT8,
  INT16,
  FLOAT32,
  COMPLEX,
};

} // namespace gimi