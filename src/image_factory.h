#pragma once

#include "model/pixel_formats.h"
#include "model/raw_image.h"
#include <iostream>
#include <vector>

using namespace std;
using namespace gimi;

class ImageFactory {
public:
  ImageFactory(uint32_t width, uint32_t height, Sampling, Interleave, uint32_t bit_depth);

public:
  gimi::RawImage create(const string &pixel_pattern = "solid");
  RawImage create_image_in_memory(string pixel_algorithm);
  vector<RawImage> create_sequence_in_memory(string pixel_algorithm);

protected:
  RawImage create_yuv_image();
  RawImage create_rgb_image();
  RawImage create_monochrome_image();

protected:
  void addChannel_rgb_interleaved_8bit(RawImage);
  void addChannel_rgb_planar_8bit(RawImage);
  void addChannel_rgb_interleaved_hdr(RawImage);
  void addChannel_yuv_444_8bit(RawImage);
  void addChannel_yuv_422_8bit(RawImage);

private:
  uint32_t m_width;
  uint32_t m_height;
  Sampling m_sampling;
  Interleave m_interleave;
  uint32_t m_bit_depth;

public:
  uint32_t m_frame_count = 5;

private:
  uint32_t m_color_1 = 0x11; // R or Y
  uint32_t m_color_2 = 0xFF; // G or U
  uint32_t m_color_3 = 0x22; // B or V
};