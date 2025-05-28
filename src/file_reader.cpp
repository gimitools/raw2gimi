#include "file_reader.h"
#include <libraw/libraw.h>

using namespace std;

void FileReader::read_file(string input_filename) {
  gimi::Image image = decode_with_libraw(input_filename);
}

gimi::Image FileReader::decode_with_libraw(const string &input_filename) {
  cout << "Decoding file: " << input_filename << endl;

  LibRaw rawProcessor;
  int ret;

  // Open File
  ret = rawProcessor.open_file(input_filename.c_str());
  check_libraw_error(ret, "Failed to open RAW file");

  // Unpack the RAW data
  ret = rawProcessor.unpack();
  check_libraw_error(ret, "Failed to unpack RAW data");

  // Process the RAW data (demosaic, gamma correction, etc.)
  ret = rawProcessor.dcraw_process();
  check_libraw_error(ret, "Failed to process RAW data");

  // Get the processed image data
  libraw_processed_image_t *libraw_image = rawProcessor.dcraw_make_mem_image(&ret);
  check_libraw_error(ret, "Failed to get processed image");

  cout << "Image decoded: " << libraw_image->width << "x" << libraw_image->height
       << ", colors: " << libraw_image->colors << ", bits: " << libraw_image->bits << endl;

  uint32_t width = libraw_image->width;
  uint32_t height = libraw_image->height;
  uint32_t bit_depth = libraw_image->bits;
  gimi::Image image(width, height, bit_depth);

  // Free the image memory
  LibRaw::dcraw_clear_mem(libraw_image);

  return image;
}

void FileReader::check_libraw_error(int ret, const string &message) {
  if (ret != LIBRAW_SUCCESS) {
    cerr << message << ": " << libraw_strerror(ret) << endl;
    exit(1);
  }
}
