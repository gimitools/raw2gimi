#include "gimifier.h"
#include "dependencies/libheif.h"
#include <cstring> // memcpy()

using namespace gimi;

void Gimifier::write_to_file(const RawImage &image, Codec codec, const string &output_filename) {
  Libheif::libheif_write_to_heif(image, codec, output_filename);
}
