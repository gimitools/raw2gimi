#pragma once

#include "model/pixel_formats.h"
#include <string>

using namespace std;

namespace gimi {

struct WriteOptions {
  string output_filename;
  Chroma chroma;
  Interleave interleave;
  Codec codec;
};
} // namespace gimi