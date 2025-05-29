#include "image_factory.h"
#include <cstring> //memset()

// Constructor
ImageFactory::ImageFactory(uint32_t width, uint32_t height, Chroma chroma, Interleave interleave, uint32_t bit_depth) {
  m_width = width;
  m_height = height;
  m_chroma = chroma;
  m_interleave = interleave;
  m_bit_depth = bit_depth;
}

// Public Functions
gimi::RawImage ImageFactory::create_image(const string &pixel_pattern) {
  RawImage image(m_width, m_height, m_bit_depth);

  switch (m_chroma) {
  case Chroma::rgb:
    create_rgb_image();
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

RawImage ImageFactory::create_yuv_image() {
  RawImage img(m_width, m_height);

  return img;
}

RawImage ImageFactory::create_rgb_image() {
  RawImage img(m_width, m_height);

  switch (m_interleave) {
  case Interleave::interleaved:
    break;
  case Interleave::planar:
    break;
  case Interleave::mixed:
  case Interleave::row:
  case Interleave::tile_component:
  case Interleave::multi_y_pixel:
    cout << "Unsupported Interleave Type: " << static_cast<int>(m_interleave) << endl;
    exit(1);
    break;
  default:
    cout << "Unrecognized Interleave Type: " << static_cast<int>(m_interleave) << endl;
    exit(1);
  }

  return img;
}

RawImage ImageFactory::create_monochrome_image() {
  cout << "Unsupported Feature!\n";
  exit(1);
}

// Protected Functions

RawImage ImageFactory::create_rgb_interleaved_8bit() {
  RawImage image(m_width, m_height, 8);

  // TODO

  return image;
}