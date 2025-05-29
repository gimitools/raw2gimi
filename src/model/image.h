#pragma once

#include "band.h"
#include <cstdint>
#include <vector>

using namespace std;

namespace gimi {

class Image {
public: // Constructor
  Image(uint32_t width, uint32_t height, uint32_t bit_depth = 8);

public:
  void add_rgb_interleaved_8bit_band(const vector<uint8_t> &data); // TODO: HDR

public: // Getters
  const vector<Band> &get_bands() const { return m_bands; }
  uint32_t get_width() const { return m_width; }
  uint32_t get_height() const { return m_height; }
  uint32_t get_bit_depth() const { return m_bit_depth; }

public:
  enum class Sampling {
    // ISO/IEC 23001-17 Table 3
    yuv_444, // no subsampling, rgb
    yuv_422,
    yuv_420,
    yuv_411,
  };

  enum class Interleave {
    // ISO/IEC 23001-17 Table 4
    planar,      // (Component Interleaving)
    interleaved, // (Pixel Interleaving)
    mixed,
    row,
    tile_component,
    multi_y_pixel
  };

protected:
  uint32_t m_width;         // Width of the image
  uint32_t m_height;        // Height of the image
  uint32_t m_bit_depth = 8; // Default bit depth
  vector<Band> m_bands;     // Data buffer for the image bands
};

} // namespace gimi