#include "gimifier.h"
#include "dependencies/libheif_wrapper.h"
#include <cstring> // memcpy()

using namespace gimi;

void Gimifier::write_to_file(const RawImage &image, WriteOptions options) {
  LibheifWrapper::write_to_heif(image, options);
}
