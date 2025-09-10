#pragma once

#include "model/pixel_formats.h"
#include <string>

using namespace std;

namespace gimi {

  struct WriteOptions {
    WriteOptions(); // Constructor
    string output_filename;
    Codec codec;
  };
} // namespace gimi