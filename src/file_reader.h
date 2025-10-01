#pragma once

#include "model/raw_image.h"
#include <iostream>
#include <libraw/libraw.h> // TODO: delete - Move libraw to it's own adaptor class
using namespace std;

class FileReader {
  // Public API
public:
  static gimi::RawImage read_file(string input_filename);
  static gimi::RawImage read_heif(string input_filename);
  static string read_text_file(string input_filename);
  static void read_csv(string input_filename);

  // Helper Functions
protected:
  static gimi::RawImage libraw_decode(const string &input_filename);
  static gimi::RawImage libraw_to_gimi(const libraw_processed_image_t *);
  static void re(int ret); // libraw error handling
};