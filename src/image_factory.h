#pragma once

#include "model/pixel_formats.h"
#include "model/raw_image.h"
#include <iostream>
#include <vector>

using namespace std;
using namespace gimi;

// TODO - Use Builder Design Pattern

class ImageFactory {
public:
  ImageFactory(uint32_t width, uint32_t height, Chroma, Interleave, PixelType);

public:
  gimi::RawImage create_image(const string &pixel_pattern = "solid");
  vector<RawImage> create_sequence(string pixel_algorithm);
  RawImageGrid create_tiles(uint32_t tile_count_x, uint32_t tile_count_y);

public:
  // Setters
  void set_width(uint32_t width);
  void set_height(uint32_t height);
  void set_frame_count(uint32_t frame_count);

protected:
  // Colorspace Functions
  RawImage create_image_yuv();
  RawImage create_image_rgb();
  RawImage create_image_mono();

protected:
  // Interleave Functions
  RawImage create_image_rgb_interleaved();
  RawImage create_image_rgb_planar();
  RawImage create_image_yuv_interleaved();
  RawImage create_image_yuv_planar();

protected:
  // Leaf Functions
  RawImage create_image_rgb_interleaved_8bit();
  RawImage create_image_rgb_interleaved_hdr();
  RawImage create_image_rgb_planar_8bit();
  RawImage create_image_444_interleaved_8bit();

protected:
  // Helper Functions
  void shuffle_colors();

private: // Member Variables
  uint32_t m_width;
  uint32_t m_height;
  Chroma m_chroma;
  Interleave m_interleave;
  PixelType m_pixel_type;
  uint32_t m_frame_count = 5;
  uint32_t m_color_1 = 0x11; // R or Y
  uint32_t m_color_2 = 0xFF; // G or U
  uint32_t m_color_3 = 0x22; // B or V
};