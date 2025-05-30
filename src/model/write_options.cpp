#include "model/write_options.h"

using namespace gimi;

WriteOptions::WriteOptions() {
  output_filename = "output.heif";
  chroma = Chroma::rgb;
  interleave = Interleave::interleaved;
  codec = Codec::raw;
  little_endian = false;
}