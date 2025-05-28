#include "band.h"
#include <cstdint>
#include <vector>

using namespace std;

namespace gimi {

class Image {
public:
  Image(uint32_t width, uint32_t height, uint32_t bit_depth = 8);

public:
  void add_rgb_interleaved_8bit_band(const vector<uint8_t> &data); // TODO: HDR

protected:
  uint32_t m_width;         // Width of the image
  uint32_t m_height;        // Height of the image
  uint32_t m_bit_depth = 8; // Default bit depth
  vector<Band> m_bands;     // Data buffer for the image bands
};

} // namespace gimi