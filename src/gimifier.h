#pragma once

#include "model/raw_image.h"
#include "model/write_options.h"
#include <iostream>

using namespace std;

namespace gimi {
class Gimifier {
public:
  static void write_to_file(const RawImage &, WriteOptions);

protected:
};
} // namespace gimi