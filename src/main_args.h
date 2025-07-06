#pragma once

#include "model/pixel_formats.h"
#include "model/raw_image.h"
#include <iostream>
#include <string>

using namespace std;

struct MainArgs {

public:
  // Constructors
  MainArgs(int argc, const char *argv[]);
  MainArgs();

public:
  // Extractors
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

public:
  // Setters
  MainArgs &set_input_filename(const string &filename);
  MainArgs &set_output_filename(const string &filename);
  MainArgs &set_output_filename();
  MainArgs &set_codec(const string &codec);
  MainArgs &set_chroma(const string &chroma);
  MainArgs &set_interleave(const string &interleave);
  MainArgs &set_pixel_type(const string &pixel_type);
  MainArgs &set_action(const string &action);
  MainArgs &set_debug(const string &debug);
  MainArgs &set_pixel_algorithm(const string &pixel_algorithm);
  MainArgs &set_width(const string &width);
  MainArgs &set_height(const string &height);
  MainArgs &set_rows(const string &rows);
  MainArgs &set_columns(const string &columns);
  MainArgs &set_tile_directory(const string &tile_directory);
  MainArgs &set_scale_factor(const string &scale_factor);

public:
  // User Input
  string input_filename;
  string output_filename;
  string codec;
  string chroma;
  string interleave;
  string pixel_type;
  string action;
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

public:
  static const string default_output_filename;

private: // Helper Functions
  uint32_t string_to_int(string str, uint32_t default_value);
  static const uint32_t default_dimensions = 64;
};
