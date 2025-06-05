#include "gimifier.h"
#include "dependencies/libheif_wrapper.h"
#include "error_handler.h"
#include <cstring> // memcpy()

using namespace gimi;

void Gimifier::write_to_file(const RawImage &image, WriteOptions options) {
  LibheifWrapper libheif(options);
  libheif.add_image(image);
  libheif.write_to_heif();
}

void Gimifier::write_to_file(vector<RawImage> &, WriteOptions) {
  throw_error("Function not yet implemented");
}