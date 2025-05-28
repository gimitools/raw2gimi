#include <cstdint>
#include <vector>

namespace gimi {

class Band {
public:
  Band(uint32_t width, uint32_t height, uint32_t bit_depth = 0);

protected:
  uint32_t m_width;          // Width of the band
  uint32_t m_height;         // Height of the band
  uint32_t m_bit_depth;      // Bit depth of the band (default is 0)
  std::vector<Band> m_bands; // Data buffer for the band
};

} // namespace gimi