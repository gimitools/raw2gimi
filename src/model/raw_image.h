#pragma once

#include "band.h"
#include "pixel_formats.h"
#include <cstdint>
#include <iostream>
#include <vector>

using namespace std;

namespace gimi {

class RawImage {
public: // Constructor
  RawImage(uint32_t width, uint32_t height, uint32_t bit_depth = 8);

public:
  void add_rgb_interleaved_8bit_band(const vector<uint8_t> &data); // TODO: HDR
  string to_string() const;

public: // Getters
  const vector<Band> &get_bands() const { return m_bands; }
  uint32_t get_width() const { return m_width; }
  uint32_t get_height() const { return m_height; }
  uint32_t get_bit_depth() const { return m_bit_depth; }
  uint32_t get_band_count() const { return m_bands.size(); }

protected:
  uint32_t m_width;         // Width of the image
  uint32_t m_height;        // Height of the image
  uint32_t m_bit_depth = 8; // Default bit depth
  vector<Band> m_bands;     // Data buffer for the image bands
  uint32_t m_band_count;    // for RGB interleaved, band_count = 3 but m_bands.size() = 1
};

} // namespace gimi