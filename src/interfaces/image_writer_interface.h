#pragma once
#include "model/pixel_formats.h"
#include "model/raw_image.h"
#include <string>

using namespace std;

namespace gimi {
class ImageWriteOptions;

class ImageWriterInterface {
public:
  virtual ~ImageWriterInterface() = default;
  virtual void write_to_file(const RawImage &image, ImageWriteOptions) = 0;
};

struct ImageWriteOptions {
  string output_filename;
  Chroma chroma;
  Interleave interleave;
  Codec codec;
};

} // namespace gimi