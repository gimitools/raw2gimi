#pragma once

#include "csv_file.h"
#include "model/iso_file.h"
#include "model/raw_image.h"
#include "model/raw_image_grid.h"
#include "model/write_options.h"
#include <iostream>
#include <vector>

using namespace std;

namespace gimi {

  // The Gimifier class writes GIMI files.
  class Gimifier {
  public:
    static void write_to_file(const IsoFile &, WriteOptions);
    static void write_to_file(const RawImage &, WriteOptions);
    static void write_grid_to_file(const RawImageGrid &, WriteOptions);
    static void write_unreal_to_rdf(const RawImageGrid &, CsvFile &, WriteOptions);
    static void write_video_to_file(vector<RawImage> &, WriteOptions);

  public:
    // Debugging
    static void debug();

  protected:
  };

} // namespace gimi