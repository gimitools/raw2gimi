#pragma once

#include <iostream>
#include <libheif/heif.h>
#include <string>

using namespace std;

class MainArgs {

public:
  MainArgs(int argc, const char *argv[]);
  MainArgs() = default;

public:
  string extract_output_filename();
  heif_compression_format extract_compression();
  heif_colorspace extract_colorspace();
  heif_chroma extract_chroma();
  uint32_t extract_bit_depth();
  uint32_t extract_width();
  uint32_t extract_height();
  uint32_t extract_rows();
  uint32_t extract_columns();
  uint32_t extract_layers();
  double extract_scale_factor();
  void print();

public:
  string input_filename;
  string output_filename;
  string encoding;
  string colorspace;
  string chroma;
  string bit_depth;
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
  const uint32_t default_bit_depth = 8;
};
