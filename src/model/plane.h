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
  Pixels data;
  BitDepth bit_depth = gimi::BitDepth::UINT8;
  size_t element_count = 0;
  uint32_t width = 0;
  uint32_t height = 0;
  // See RawImage for Interleave and Chroma

  Plane(Pixels, uint32_t width, uint32_t height, BitDepth);
};

} // namespace gimi