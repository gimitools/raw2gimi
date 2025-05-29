#pragma once

namespace gimi {

enum class Chroma {
  rgb,
  monochrome,
  yuv_444,
  yuv_422,
  yuv_420,
  yuv_411,
};

enum class Interleave {
  planar,
  interleaved,
  mixed,
  row,
  tile_component,
  multi_y_pixel
};

} // namespace gimi