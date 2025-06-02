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

using Pixels = std::variant<
    vector<uint8_t>,
    vector<uint16_t>,
    vector<int16_t>,
    vector<float>,
    vector<complex<float>>>;

struct Plane {
  Pixels m_pixels;
  BitDepth m_bit_depth = gimi::BitDepth::UINT8;
  uint32_t m_width = 0;
  uint32_t m_height = 0;
  // Interleave m_interleave; // See RawImage for Interleave
  // Chroma m_chroma;         // See RawImage for Chroma

  Plane(Pixels, uint32_t width, uint32_t height, BitDepth);
};

} // namespace gimi