#include "band.h"
#include <cstdint>
#include <vector>

namespace gimi {

class Image {
public:
  Image(uint32_t width, uint32_t height, uint32_t bit_depth = 8);

protected:
  uint32_t m_width;         // Width of the image
  uint32_t m_height;        // Height of the image
  uint32_t m_bit_depth = 8; // Default bit depth
};

} // namespace gimi