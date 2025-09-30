#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "error_handler.h"
#include "stb_image_wrapper.h"
#include <stdexcept>

using namespace gimi;

RawImage StbImageWrapper::load(const std::string &filename) {
  int width, height, channels;
  unsigned char *raw = stbi_load(filename.c_str(), &width, &height, &channels, 0);
  if (!raw) {
    throw std::runtime_error("Failed to load image: " + filename);
  }

  // Copy into a vector (so we don’t depend on stb’s malloc/free)
  size_t size = static_cast<size_t>(width) * height * channels;
  RawImage img(width, height);

  if (channels == 3) {
    img.add_rgb_interleaved_8bit(vector<uint8_t>(raw, raw + size));
  } else if (channels == 4) {
    img.add_rgba_interleaved_8bit(vector<uint8_t>(raw, raw + size));
  } else {
    // print the first 64 bytes of raw for debugging
    for (size_t i = 0; i < std::min(size, static_cast<size_t>(64)); i++) {
      printf("%02x ", raw[i]);
    }
    printf("\n");
    throw_error("Unsupported number of channels: %d", channels);
  }
  // StbImageWrapper::Image img;
  // img.width = width;
  // img.height = height;
  // img.channels = channels;
  // img.data.assign(raw, raw + size);

  // Free stb's memory
  stbi_image_free(raw);

  return img;
}
