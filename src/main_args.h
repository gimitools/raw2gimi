#pragma once

#include "model/pixel_formats.h"
#include "model/raw_image.h"
#include <iostream>
#include <string>

using namespace std;

class MainArgs {

public:
  MainArgs(int argc, const char *argv[]);
  MainArgs() = default;

public:
  string extract_output_filename();
  gimi::Codec extract_codec();
  gimi::Chroma extract_chroma();
  gimi::Interleave extract_interleave();
  gimi::PixelType extract_pixel_type();
  uint32_t extract_width();
  uint32_t extract_height();
  uint32_t extract_rows();
  uint32_t extract_columns();
  uint32_t extract_layers();
  double extract_scale_factor();
  void print();

public: // User Input
  string input_filename;
  string output_filename;
  string codec;
  string chroma;
  string interleave;
  string pixel_type;
  string action = "heif_to_gimi";
  string debug;
  string pixel_algorithm;
  string width;
  string height;
  string rows;
  string columns;
  string tile_directory;
  string scale_factor;
  string layers;
  bool flag1;

private: // Helper Functions
  uint32_t string_to_int(string str, uint32_t default_value);
  const uint32_t default_dimensions = 64;
};
