#include "raw_image.h"

using namespace gimi;

RawImage::RawImage(uint32_t width, uint32_t height, uint32_t bit_depth) {
  m_width = width;
  m_height = height;
  m_bit_depth = bit_depth;
}

string RawImage::to_string() const {
  return "RawImage: " + std::to_string(m_width) + "x" + std::to_string(m_height) + "@" + std::to_string(m_bit_depth) + "bit";
}

void gimi::RawImage::add_rgb_interleaved_8bit_band(const vector<uint8_t> &data) {
  Band band(m_width, m_height, 8);
  band.set_rgb_interleaved_8bit_data(data);
  m_bands.push_back(band);
  m_band_count = 3;
}