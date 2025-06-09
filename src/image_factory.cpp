#include "image_factory.h"
#include "error_handler.h"
#include "pixel_formats.h"
#include <cstring> //memset()

// Constructor
ImageFactory::ImageFactory(uint32_t width, uint32_t height, Chroma chroma, Interleave interleave, PixelType pixel_type) {
  m_width = width;
  m_height = height;
  m_chroma = chroma;
  m_interleave = interleave;
  m_pixel_type = pixel_type;
}

// Public Functions
RawImage ImageFactory::create_image(const string &pixel_pattern) {

  switch (m_chroma) {
  case Chroma::rgb:
    return create_image_rgb();
  case Chroma::gray:
    throw_error("Unsupported Feature: Monochrome Image Creation!");
  case Chroma::yuv_444:
  case Chroma::yuv_422:
  case Chroma::yuv_420:
  case Chroma::yuv_411:
    return create_image_yuv();
  default:
    throw_error("Unsupported Chroma Format: %s", to_string(m_chroma).c_str());
  }

  throw_error("");
  RawImage image(m_width, m_height);
  return image;
}

vector<RawImage> ImageFactory::create_sequence(string pixel_algorithm) {
  vector<RawImage> images;

  for (uint32_t i = 0; i < m_frame_count; i++) {
    RawImage img = create_image(pixel_algorithm);
    images.push_back(img);
    shuffle_colors();
  }

  return images;
}

vector<RawImage> ImageFactory::create_tiles(uint32_t tile_count_x, uint32_t tile_count_y) {
  vector<RawImage> tiles;
  throw_error("Function not yet implemented");
  return tiles;
}

// Colorspace Functions

RawImage ImageFactory::create_image_yuv() {
  switch (m_interleave) {
  case Interleave::interleaved:
    return create_image_yuv_interleaved();
  case Interleave::planar:
    return create_image_yuv_planar();
  default:
    throw_error("Unsupported Interleave Type: %s", to_string(m_interleave).c_str());
  }
  RawImage image(0, 0);
  return image;
}

RawImage ImageFactory::create_image_rgb() {
  switch (m_interleave) {
  case Interleave::interleaved:
    return create_image_rgb_interleaved();
  case Interleave::planar:
    return create_image_rgb_planar();
    break;
  default:
    throw_error("Unsupported Interleave Type: %s", to_string(m_interleave).c_str());
  }
  throw_error();
  RawImage image(0, 0);
  return image;
}

RawImage ImageFactory::create_image_mono() {
  throw_error("Unsupported Feature: Monochrome Image Creation!");
  return RawImage(0, 0);
}

// Interleave Functions

RawImage ImageFactory::create_image_rgb_interleaved() {
  switch (m_pixel_type) {
  case PixelType::uint8:
    return create_image_rgb_interleaved_8bit();
  case PixelType::uint10:
  case PixelType::uint12:
  case PixelType::uint14:
  case PixelType::uint16:
    return create_image_rgb_interleaved_hdr();
  case PixelType::int8:
  case PixelType::int16:
  case PixelType::float32:
  case PixelType::complex:
  case PixelType::mixed:
  default:
    throw_error("Unsupported Pixel Type: %s", to_string(m_pixel_type).c_str());
  }
  return RawImage(0, 0);
}

RawImage ImageFactory::create_image_rgb_planar() {
  switch (m_pixel_type) {
  case PixelType::uint8:
    return create_image_rgb_planar_8bit();
  case PixelType::uint10:
  case PixelType::uint12:
  case PixelType::uint14:
  case PixelType::uint16:
  case PixelType::int8:
  case PixelType::int16:
  case PixelType::float32:
  case PixelType::complex:
  case PixelType::mixed:
  default:
    throw_error("Unsupported Pixel Type: %s", to_string(m_pixel_type).c_str());
  }
  return RawImage(0, 0);
}

RawImage ImageFactory::create_image_yuv_interleaved() {
  switch (m_pixel_type) {
  case PixelType::uint8:
    return create_image_444_interleaved_8bit();
  case PixelType::uint10:
  case PixelType::uint12:
  case PixelType::uint14:
  case PixelType::uint16:
  case PixelType::int8:
  case PixelType::int16:
  case PixelType::float32:
  case PixelType::complex:
  case PixelType::mixed:
  default:
    throw_error("Unsupported Pixel Type: %s", to_string(m_pixel_type).c_str());
  }
  return RawImage(0, 0);
}

RawImage ImageFactory::create_image_yuv_planar() {
  // Variables
  RawImage image(m_width, m_height);
  const uint32_t band_count = 3;     // RGB
  const uint32_t bytes_per_band = 1; // 8-bit per channel
  uint64_t size_in_bytes = m_width * m_height * bytes_per_band;
  vector<uint8_t> Y;
  vector<uint8_t> u;
  vector<uint8_t> v;
  Y.reserve(size_in_bytes);
  u.reserve(size_in_bytes);
  v.reserve(size_in_bytes);

  // Fill Pixels
  for (uint32_t y = 0; y < m_height; y++) {
    for (uint32_t x = 0; x < m_width; x++) {
      Y.push_back(static_cast<uint8_t>(m_color_1)); // R or Y
      u.push_back(static_cast<uint8_t>(m_color_2)); // G or U
      v.push_back(static_cast<uint8_t>(m_color_3)); // B or V
    }
  }

  // Add Pixels
  image.add_yuv_444_planar_8bit(Y, u, v);

  return image;
}

// Leaf Functions

RawImage ImageFactory::create_image_rgb_interleaved_8bit() {
  // Variables
  RawImage image(m_width, m_height);
  const uint32_t band_count = 3;     // RGB
  const uint32_t bytes_per_band = 1; // 8-bit per channel
  uint64_t size = m_width * m_height * band_count * bytes_per_band;
  vector<uint8_t> pixels;
  pixels.reserve(size);

  // Fill Pixels
  for (uint32_t y = 0; y < m_height; y++) {
    for (uint32_t x = 0; x < m_width; x++) {
      pixels.push_back(static_cast<uint8_t>(m_color_1)); // R or Y
      pixels.push_back(static_cast<uint8_t>(m_color_2)); // G or U
      pixels.push_back(static_cast<uint8_t>(m_color_3)); // B or V
    }
  }

  // Add Pixels
  image.add_rgb_interleaved_8bit(pixels);

  return image;
}

RawImage ImageFactory::create_image_rgb_interleaved_hdr() {
  // Variables
  RawImage image(m_width, m_height);
  const uint32_t band_count = 3; // RGB
  const uint32_t bytes_per_pixel = 2;
  uint64_t pixel_count = m_width * m_height * band_count;
  const uint64_t size_in_bytes = pixel_count * bytes_per_pixel;
  vector<uint8_t> pixels;
  pixels.reserve(size_in_bytes);

  // Fill Pixels
  for (uint32_t y = 0; y < m_height; y++) {
    for (uint32_t x = 0; x < m_width; x++) {
      pixels.push_back(static_cast<uint8_t>(m_color_1)); // R or Y
      pixels.push_back(static_cast<uint8_t>(m_color_1)); // R or Y

      pixels.push_back(static_cast<uint8_t>(m_color_2)); // G or U
      pixels.push_back(static_cast<uint8_t>(m_color_2)); // G or U

      pixels.push_back(static_cast<uint8_t>(m_color_3)); // B or V
      pixels.push_back(static_cast<uint8_t>(m_color_3)); // B or V
    }
  }

  // Add Pixels
  image.add_rgb_interleaved_hdr(pixels, m_pixel_type);

  return image;
}

RawImage ImageFactory::create_image_rgb_planar_8bit() {
  // Variables
  RawImage image(m_width, m_height);
  const uint32_t band_count = 3;     // RGB
  const uint32_t bytes_per_band = 1; // 8-bit per channel
  uint64_t size_in_bytes = m_width * m_height * bytes_per_band;
  vector<uint8_t> r;
  vector<uint8_t> g;
  vector<uint8_t> b;
  r.reserve(size_in_bytes);
  g.reserve(size_in_bytes);
  b.reserve(size_in_bytes);

  // Fill Pixels
  for (uint32_t y = 0; y < m_height; y++) {
    for (uint32_t x = 0; x < m_width; x++) {
      r.push_back(static_cast<uint8_t>(m_color_1)); // R or Y
      g.push_back(static_cast<uint8_t>(m_color_2)); // G or U
      b.push_back(static_cast<uint8_t>(m_color_3)); // B or V
    }
  }

  // Add Pixels
  image.add_rgb_planar_8bit(r, g, b);

  return image;
}

RawImage ImageFactory::create_image_444_interleaved_8bit() {
  // Variables
  RawImage image(m_width, m_height);
  const uint32_t band_count = 3;     // RGB
  const uint32_t bytes_per_band = 1; // 8-bit per channel
  uint64_t size = m_width * m_height * band_count * bytes_per_band;
  vector<uint8_t> pixels;
  pixels.reserve(size);

  // Fill Pixels
  for (uint32_t y = 0; y < m_height; y++) {
    for (uint32_t x = 0; x < m_width; x++) {
      pixels.push_back(static_cast<uint8_t>(m_color_1)); // R or Y
      pixels.push_back(static_cast<uint8_t>(m_color_2)); // G or U
      pixels.push_back(static_cast<uint8_t>(m_color_3)); // B or V
    }
  }

  // Add Pixels
  image.add_yuv_444_interleaved_8bit(pixels);

  return image;
}

// Helper Functions

void ImageFactory::shuffle_colors() {
  uint32_t temp = m_color_1;
  m_color_1 = m_color_2;
  m_color_2 = m_color_3;
  m_color_3 = temp;
}