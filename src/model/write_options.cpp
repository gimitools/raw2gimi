#include "model/write_options.h"

using namespace gimi;

WriteOptions::WriteOptions() {
  output_filename = "output.heif";
  codec = Codec::raw;
}