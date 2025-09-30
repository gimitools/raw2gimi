#pragma once

#include "model/raw_image.h"
#include "model/raw_image_grid.h"

using namespace std;

namespace gimi {

  class ImageProcessor {

  public:
    static RawImageGrid image_to_grid(const RawImage &image, uint32_t rows, uint32_t columns);

  private:
    static RawImageGrid yuv_444_8bit_to_grid(const RawImage &image, uint32_t rows, uint32_t columns);
    static RawImageGrid rgb_interleaved_8bit_to_grid(const RawImage &image, uint32_t rows, uint32_t columns);
    static Plane extract_tile(const Plane &plane, uint32_t x_start, uint32_t y_start, uint32_t tile_width, uint32_t tile_height);
    static Plane extract_tile_interleaved(const Plane &plane, uint32_t x_start, uint32_t y_start, uint32_t tile_width, uint32_t tile_height, uint32_t band_count);
  };

} // namespace gimi