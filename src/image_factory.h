#pragma once

#include "model/pixel_formats.h"
#include "model/raw_image.h"
#include <iostream>
#include <vector>

using namespace std;
using namespace gimi;

class ImageFactory {
public:
  ImageFactory(uint32_t width, uint32_t height, Chroma, Interleave, BitDepth);

public:
  gimi::RawImage create_image(const string &pixel_pattern = "solid");
  vector<RawImage> create_sequence_in_memory(string pixel_algorithm);

protected:
  RawImage create_yuv_image();
  RawImage create_rgb_image();
  RawImage create_monochrome_image();

protected:
  RawImage create_rgb_interleaved_8bit();

private: // Member Variables
  uint32_t m_width;
  uint32_t m_height;
  Chroma m_chroma;
  Interleave m_interleave;
  BitDepth m_bit_depth;

public:
  uint32_t m_frame_count = 5; // TODO: don't show your private parts in public

private:
  uint32_t m_color_1 = 0x11; // R or Y
  uint32_t m_color_2 = 0xFF; // G or U
  uint32_t m_color_3 = 0x22; // B or V
};