#include "gimifier.h"
#include "dependencies/libheif_wrapper.h"
#include <cstring> // memcpy()

using namespace gimi;

void Gimifier::write_to_file(const RawImage &image, WriteOptions options) {
  LibheifWrapper::write_to_heif(image, options);
}

void Gimifier::write_to_file(vector<RawImage> &, WriteOptions) {
  cout << "TODO: write_to_file(vector<RawImage> &)" << endl;
  exit(1);
}