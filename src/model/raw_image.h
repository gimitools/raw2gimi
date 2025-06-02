#pragma once

#include "model/pixel_formats.h"
#include "model/plane.h"

#include <complex>
#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

using namespace std;

namespace gimi {

class RawImage {
public:
  RawImage(uint32_t w, uint32_t h);

public:
  uint32_t get_width() const { return width; }
  uint32_t get_height() const { return height; }
  uint32_t get_bit_depth() const { return 8; }
  uint32_t get_band_count() const { return 3; }

private:
  uint32_t width = 0;
  uint32_t height = 0;
  Interleave interleave = Interleave::planar;
  Chroma chroma = Chroma::rgb;
  vector<Plane> planes;
};

} // namespace gimi
