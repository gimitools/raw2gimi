#include "plane.h"

using namespace gimi;

Plane::Plane(vector<uint8_t> pixels, uint32_t width, uint32_t height, PixelType pixel_type) {
  m_pixels = pixels;
  m_pixel_type = pixel_type;
  m_width = width;
  m_height = height;
}
