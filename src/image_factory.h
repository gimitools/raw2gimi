#pragma once

#include "model/raw_image.h"
#include <iostream>
#include <libheif/heif.h> // Delete: should not depend on libheif
#include <vector>

using namespace std;
using namespace gimi;

class ImageFactory {
public:
  ImageFactory(uint32_t width, uint32_t height, heif_chroma chroma, heif_colorspace colorspace, uint32_t bit_depth);

public:
  gimi::RawImage create(const string &pixel_pattern = "solid");
  heif_image *create_image_in_memory(string pixel_algorithm);
  vector<heif_image *> create_sequence_in_memory(string pixel_algorithm);

protected:
  // TODO - don't duplicate this function
  // class UseLibheif or LibheifUtils #include that instead of libheif
  static void he(struct heif_error);

protected:
  heif_image *create_yuv_image();
  heif_image *create_rgb_image();
  heif_image *create_monochrome_image();

protected:
  void addChannel_rgb_interleaved_8bit(heif_image *img);
  void addChannel_rgb_planar_8bit(heif_image *img);
  void addChannel_rgb_interleaved_hdr(heif_image *img);
  void addChannel_yuv_444_8bit(heif_image *img);
  void addChannel_yuv_422_8bit(heif_image *img);

private:
  uint32_t m_width;
  uint32_t m_height;
  heif_chroma m_chroma;
  heif_colorspace m_colorspace;
  uint32_t m_bit_depth;

public:
  uint32_t m_frame_count = 5;

private:
  uint32_t m_color_1 = 0x11; // R or Y
  uint32_t m_color_2 = 0xFF; // G or U
  uint32_t m_color_3 = 0x22; // B or V
};