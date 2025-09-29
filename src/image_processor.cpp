#include "image_processor.h"
#include "error_handler.h"

using namespace gimi;

RawImageGrid ImageProcessor::image_to_grid(const RawImage &image, uint32_t rows, uint32_t columns) {
  uint32_t tile_width = image.get_width() / columns;
  uint32_t tile_height = image.get_height() / rows;

  if (image.get_chroma() == Chroma::yuv_444 &&
      image.get_pixel_type() == PixelType::uint8 &&
      image.get_interleave() == Interleave::planar) {
    ImageProcessor::yuv_444_8bit_to_grid(image, rows, columns);
  } else {
    throw_error("Unsupported image format for tiling");
  }

  RawImageGrid grid;

  return grid;
}

RawImageGrid ImageProcessor::yuv_444_8bit_to_grid(const RawImage &image, uint32_t rows, uint32_t columns) {
  RawImageGrid grid;
  uint32_t tile_width = image.get_width() / columns;
  uint32_t tile_height = image.get_height() / rows;
  const vector<Plane> planes = image.get_planes();

  if (planes.size() != 3) {
    throw_error("Expected 3 planes for YUV 444, but got: %zu", planes.size());
  }

  const vector<uint8_t> &plane_y = planes[0].m_pixels;
  const vector<uint8_t> &plane_u = planes[1].m_pixels;
  const vector<uint8_t> &plane_v = planes[2].m_pixels;

  for (uint32_t row = 0; row < rows; row++) {
    for (uint32_t column = 0; column < columns; column++) {
      uint32_t x_start = column * tile_width;
      uint32_t y_start = row * tile_height;
      RawImage tile(tile_width, tile_height);

      Plane tile_y = extract_tile(planes[0], x_start, y_start, tile_width, tile_height);
      Plane tile_u = extract_tile(planes[1], x_start, y_start, tile_width, tile_height);
      Plane tile_v = extract_tile(planes[2], x_start, y_start, tile_width, tile_height);
    }
  }

  return grid;
}

Plane ImageProcessor::extract_tile(const Plane &plane, uint32_t x_start, uint32_t y_start, uint32_t tile_width, uint32_t tile_height) {
  vector<uint8_t> tile_pixels;
  uint32_t bytes_per_pixel = gimi::get_bit_depth(plane.m_pixel_type) / 8;
  uint32_t stride = plane.m_width * bytes_per_pixel;

  for (uint32_t y = 0; y < tile_height; y++) {
    uint32_t src_start = (y_start + y) * stride + x_start * bytes_per_pixel;
    uint32_t src_end = src_start + tile_width * bytes_per_pixel;
    if (src_end > plane.m_pixels.size()) {
      throw_error("Tile extraction out of bounds");
    }
    tile_pixels.insert(tile_pixels.end(), plane.m_pixels.begin() + src_start, plane.m_pixels.begin() + src_end);
  }

  return Plane(tile_pixels, tile_width, tile_height, plane.m_pixel_type);
}