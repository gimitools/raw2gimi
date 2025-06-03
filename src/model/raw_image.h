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
  // Constructor
  RawImage(uint32_t w, uint32_t h);

public:
  // Getters
  uint32_t get_width() const;
  uint32_t get_height() const;
  uint32_t get_bit_depth() const;
  uint32_t get_band_count() const;
  const vector<Plane> &get_planes() const;

public:
  // API
  void add_rgb_interleaved_8bit(const vector<uint8_t> &);
  void add_rgb_interleaved_hdr(const vector<uint16_t> &, PixelType);

private:
  // Pixel Data
  vector<Plane> planes;

private:
  // Metadata
  uint32_t width;
  uint32_t height;
  Interleave interleave = Interleave::planar;
  Chroma chroma = Chroma::rgb;
  PixelType pixel_type = PixelType::uint8;
  bool little_endian = false;
};

} // namespace gimi
