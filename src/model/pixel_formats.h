#pragma once

namespace gimi {

// ISO/IEC 23001-17 Table 3
enum class Sampling {
  yuv_444, // no subsampling & rgb
  yuv_422,
  yuv_420,
  yuv_411,
};

// ISO/IEC 23001-17 Table 4
enum class Interleave {
  planar,      // (Component Interleaving)
  interleaved, // (Pixel Interleaving)
  mixed,
  row,
  tile_component,
  multi_y_pixel
};

} // namespace gimi