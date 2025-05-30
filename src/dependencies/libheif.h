#pragma once

#include "model/pixel_formats.h"
#include "model/raw_image.h"
#include <string>
using namespace std;
using namespace gimi;

class Libheif {

public:
  static void write_to_heif(const RawImage &image, Codec, const string &output_filename);

private:
  static void he(struct heif_error);
};