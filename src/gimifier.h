#pragma once

#include "model/raw_image.h"
#include <iostream>

using namespace std;

namespace gimi {
class Gimifier {
public:
  static void write_to_file(const RawImage &, const string &output_filename);

protected:
  static void libheif_write_to_heif(const RawImage &image, const string &output_filename);
};
} // namespace gimi