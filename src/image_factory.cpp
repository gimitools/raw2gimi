#include "image_factory.h"
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
gimi::RawImage ImageFactory::create_image(const string &pixel_pattern) {

  switch (m_chroma) {
  case Chroma::rgb:
    return create_image_rgb();
    break;
  case Chroma::gray:
    cout << "Unsupported Feature: Monochrome Image Creation!\n";
    exit(1);
    break;
  case Chroma::yuv_444:
    cout << "unsupported Feature: YUV 444 Image Creation!\n";
    exit(1);
    break;
  case Chroma::yuv_422:
    cout << "Unsupported Feature: YUV 422 Image Creation!\n";
    exit(1);
    break;
  case Chroma::yuv_420:
    cout << "Unsupported Feature: YUV 420 Image Creation!\n";
    exit(1);
    break;
  case Chroma::yuv_411:
    cout << "Unsupported Feature: YUV 411 Image Creation!\n";
    exit(1);
    break;
  default:
    cout << "Unsupported Chroma Format: " << to_string(m_chroma) << endl;
    exit(1);
  }

  cerr << "ImageFactory::create_image() failed!" << endl;
  exit(1);
  RawImage image(m_width, m_height);
  return image;
}

vector<RawImage> ImageFactory::create_sequence_in_memory(string pixel_algroithm) {
  vector<RawImage> images;

  for (uint32_t i = 0; i < m_frame_count; i++) {
    RawImage img = create_image(pixel_algroithm);
    images.push_back(img);

    // Shuffle Colors
    uint32_t temp = m_color_1;
    m_color_1 = m_color_2;
    m_color_2 = m_color_3;
    m_color_3 = temp;
  }

  return images;
}

// Protected Functions

RawImage ImageFactory::create_image_yuv() {
  RawImage img(m_width, m_height);

  return img;
}

RawImage ImageFactory::create_image_rgb() {

  switch (m_interleave) {
  case Interleave::interleaved:
    return create_image_rgb_interleaved();
  case Interleave::planar:
    return create_image_rgb_planar();
    break;
  default:
    cout << "Unrecognized Interleave Type: " << to_string(m_interleave) << endl;
    exit(1);
  }

  cerr << "ImageFactory::create_image_rgb() failed!" << endl;
  exit(1);
  RawImage image(m_width, m_height);
  return image;
}

RawImage ImageFactory::create_image_mono() {
  cout << "Unsupported Feature!\n";
  exit(1);
}

// Protected Functions

RawImage ImageFactory::create_image_rgb_interleaved() {
  switch (m_pixel_type) {
  case PixelType::uint8:
    return create_image_rgb_interleaved_8bit();
  case PixelType::uint10:
    return create_image_rgb_interleaved_10bit();
  case PixelType::uint12:
    return create_image_rgb_interleaved_hdr();
  case PixelType::uint14:
  case PixelType::uint16:
  case PixelType::int8:
  case PixelType::int16:
  case PixelType::float32:
  case PixelType::complex:
  case PixelType::mixed:
  default:
    cout << __func__ << "(): Unsupported Bit Depth: " << to_string(m_pixel_type) << endl;
    exit(1);
  }
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
    cout << __func__ << "(): Unsupported Bit Depth: " << to_string(m_pixel_type) << endl;
    exit(1);
  }
  cout << "Unsupported Feature: Planar RGB Image Creation!\n";
  exit(1);
}

// Protected Functions

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

RawImage ImageFactory::create_image_rgb_interleaved_10bit() {
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
  image.add_rgb_interleaved_hdr(pixels, PixelType::uint10);

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