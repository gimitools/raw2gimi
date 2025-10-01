#pragma once

#include "model/pixel_formats.h"
#include <string>

using namespace std;

namespace gimi {

  // Write Options for a Single Image, tile, or sequence.
  // Not to be confused with MainArgs which is for CLI arguments.
  struct WriteOptions {
    WriteOptions(); // Constructor
    string output_filename;
    Codec codec;
    uint32_t rows;
    uint32_t columns;
    string image_name;
  };
} // namespace gimi