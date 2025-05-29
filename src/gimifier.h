#pragma once

#include "model/raw_image.h"
#include <iostream>

using namespace std;

namespace gimi {
class Gimifier {
public:
  static void write_to_file(const RawImage &, Codec, const string &output_filename);

protected:
};
} // namespace gimi