#pragma once
#include "model/pixel_formats.h"
#include "model/raw_image.h"
#include "model/write_options.h"
#include <string>

using namespace std;

namespace gimi {

class ImageWriterInterface {
public:
  virtual ~ImageWriterInterface() = default;
  virtual void write_to_file(const RawImage &image, WriteOptions) = 0;
};

} // namespace gimi