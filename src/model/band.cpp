#include "band.h"

gimi::Band::Band(uint32_t width, uint32_t height, uint32_t bit_depth) {
  m_width = width;
  m_height = height;
  m_bit_depth = bit_depth;
  // Constructor implementation
}

void gimi::Band::set_rgb_interleaved_8bit_data(const vector<uint8_t> &data) {
  m_data = data;
}