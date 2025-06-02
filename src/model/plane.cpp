#include "plane.h"

using namespace gimi;

Plane::Plane(Pixels pixels, uint32_t width, uint32_t height, BitDepth bit_depth) {
  m_pixels = pixels;
  m_bit_depth = bit_depth;
  m_width = width;
  m_height = height;
}
