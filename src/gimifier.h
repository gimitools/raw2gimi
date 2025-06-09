#pragma once

#include "model/raw_image.h"
#include "model/write_options.h"
#include <iostream>
#include <vector>

using namespace std;

namespace gimi {

// The Gimifier class writes GIMI files.
class Gimifier {
public:
  static void write_to_file(const RawImage &, WriteOptions);
  static void write_grid_to_file(vector<RawImage> &, WriteOptions);
  static void write_video_to_file(vector<RawImage> &, WriteOptions);

protected:
};

} // namespace gimi