#pragma once

#include "model/pixel_formats.h"
#include <complex>
#include <cstdint>
#include <iostream>
#include <map>
#include <string>
#include <variant>
#include <vector>

using namespace std;

namespace gimi {

struct Plane {
  vector<uint8_t> m_pixels;
  PixelType m_pixel_type = gimi::PixelType::uint8;
  uint32_t m_width = 0;
  uint32_t m_height = 0;
  // Should not varry across planes:
  //   1. Interleave m_interleave (should a Plane know how it's interleaved?)
  //   2. Chroma, (describes a set of planes).

  Plane(vector<uint8_t> pixels, uint32_t width, uint32_t height, PixelType);
};

} // namespace gimi