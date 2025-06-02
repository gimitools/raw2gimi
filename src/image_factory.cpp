#include "image_factory.h"
#include <cstring> //memset()

// Constructor
ImageFactory::ImageFactory(uint32_t width, uint32_t height, Chroma chroma, Interleave interleave, BitDepth bit_depth) {
  m_width = width;
  m_height = height;
  m_chroma = chroma;
  m_interleave = interleave;
  m_bit_depth = bit_depth;
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
    cout << "Unsupported Chroma Format: " << static_cast<int>(m_chroma) << endl;
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
    cout << "Unsupported Feature: Planar RGB Image Creation!\n";
    exit(1);
    break;
  default:
    cout << "Unrecognized Interleave Type: " << static_cast<int>(m_interleave) << endl;
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
  switch (m_bit_depth) {
  case BitDepth::uint8:
    return create_image_rgb_interleaved_8bit();
  case BitDepth::uint10:
  case BitDepth::uint12:
  case BitDepth::uint14:
  case BitDepth::uint16:
  case BitDepth::int8:
  case BitDepth::int16:
  case BitDepth::float32:
  case BitDepth::complex:
  case BitDepth::mixed:
  default:
    cout << "Unsupported Bit Depth: " << static_cast<int>(m_bit_depth) << endl;
    exit(1);
  }
}

// Protected Functions

RawImage ImageFactory::create_image_rgb_interleaved_8bit() {

  // Variables
  RawImage image(m_width, m_height);
  const uint32_t band_count = 3; // RGB
  uint64_t size = m_width * m_height * band_count;
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