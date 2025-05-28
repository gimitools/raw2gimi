#pragma once

#include "model/image.h"
#include <iostream>

using namespace std;

namespace gimi {
class FileWriter {
public:
  static void write_image_to_file(const Image &, const string &output_filename);

protected:
  static void libheif_write_to_heif(const Image &image, const string &output_filename);
};
} // namespace gimi