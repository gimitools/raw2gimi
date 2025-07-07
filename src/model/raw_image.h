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
  Interleave get_interleave() const;
  PixelType get_pixel_type() const;
  Chroma get_chroma() const;
  bool is_little_endian() const;
  const vector<Plane> &get_planes() const;

public:
  // API
  void add_rgb_interleaved_8bit(const vector<uint8_t> &);
  void add_rgb_interleaved_hdr(const vector<uint8_t> &, PixelType);
  void add_rgb_planar_8bit(const vector<uint8_t> &r,
                           const vector<uint8_t> &g,
                           const vector<uint8_t> &b);
  void add_yuv_444_interleaved_8bit(const vector<uint8_t> &pixels);
  void add_yuv_444_planar_8bit(const vector<uint8_t> &y,
                               const vector<uint8_t> &u,
                               const vector<uint8_t> &v);
  void add_mono_8bit(const vector<uint8_t> &pixels);
  void add_mono_16bit(const vector<uint8_t> &pixels);

private:
  // Pixel Data
  vector<Plane> planes;

private:
  // Metadata
  uint32_t m_width;
  uint32_t m_height;
  Interleave m_interleave = Interleave::planar;
  Chroma m_chroma = Chroma::rgb;
  PixelType m_pixel_type = PixelType::uint8;
  bool little_endian = false;
};

using RawImageGrid = vector<vector<RawImage>>;

} // namespace gimi
