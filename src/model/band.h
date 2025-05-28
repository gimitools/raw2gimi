#pragma once

#include <cstdint>
#include <vector>

using namespace std;

namespace gimi {

class Band {
public:
  Band(uint32_t width, uint32_t height, uint32_t bit_depth = 8);

public:
  void set_rgb_interleaved_8bit_data(const vector<uint8_t> &data);

protected:
public:
  uint32_t m_width;       // Width of the band
  uint32_t m_height;      // Height of the band
  uint32_t m_bit_depth;   // Bit depth of the band
  vector<uint8_t> m_data; // Data buffer for the band
};

} // namespace gimi