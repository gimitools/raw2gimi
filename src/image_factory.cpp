#include "image_factory.h"
#include <cstring> //memset()

// Constructor
ImageFactory::ImageFactory(uint32_t width, uint32_t height, Sampling sampling, Interleave interleave, uint32_t bit_depth) {
  m_width = width;
  m_height = height;
  m_sampling = sampling;
  m_interleave = interleave;
  m_bit_depth = bit_depth;
}

// Public Functions
gimi::RawImage ImageFactory::create(const string &pixel_pattern) {
  cout << "TODO: ImageFactory::create()" << endl;
  exit(1);
}

RawImage ImageFactory::create_image_in_memory(string pixel_algorithm) {

  RawImage img(m_width, m_height);

  return img;
}

vector<RawImage> ImageFactory::create_sequence_in_memory(string pixel_algroithm) {
  vector<RawImage> images;

  for (uint32_t i = 0; i < m_frame_count; i++) {
    RawImage img = create_image_in_memory(pixel_algroithm);
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

  return img;
}

RawImage ImageFactory::create_monochrome_image() {
  cout << "Unsupported Feature!\n";
  exit(1);
}

// Protected Functions

void ImageFactory::addChannel_rgb_interleaved_8bit(RawImage img) {
}

void ImageFactory::addChannel_rgb_planar_8bit(RawImage img) {
}

void ImageFactory::addChannel_rgb_interleaved_hdr(RawImage img) {
}

void ImageFactory::addChannel_yuv_444_8bit(RawImage img) {
}

void ImageFactory::addChannel_yuv_422_8bit(RawImage img) {
}