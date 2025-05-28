#include "file_reader.h"
#include <libraw/libraw.h>

using namespace std;

void FileReader::read_file(string input_filename) {
  decode_with_libraw(input_filename);
}

void FileReader::decode_with_libraw(string input_filename) {
  cout << "Decoding file: " << input_filename << endl;

  LibRaw rawProcessor;
  int ret;

  // Open the RAW file
  ret = rawProcessor.open_file(input_filename.c_str());
  if (ret != LIBRAW_SUCCESS) {
    cerr << "Failed to open file: " << libraw_strerror(ret) << endl;
    exit(1);
  }

  // Unpack the RAW data
  ret = rawProcessor.unpack();
  if (ret != LIBRAW_SUCCESS) {
    cerr << "Failed to unpack file: " << libraw_strerror(ret) << endl;
    exit(1);
  }

  // Process the RAW data (demosaic, gamma correction, etc.)
  ret = rawProcessor.dcraw_process();
  if (ret != LIBRAW_SUCCESS) {
    cerr << "Failed to process RAW data: " << libraw_strerror(ret) << endl;
    exit(1);
  }

  // Get the processed image data
  libraw_processed_image_t *libraw_image = rawProcessor.dcraw_make_mem_image(&ret);
  if (!libraw_image || ret != LIBRAW_SUCCESS) {
    cerr << "Failed to get processed image: " << libraw_strerror(ret) << endl;
    exit(1);
  }

  cout << "Image decoded: " << libraw_image->width << "x" << libraw_image->height
       << ", colors: " << libraw_image->colors << ", bits: " << libraw_image->bits << endl;

  // Free the image memory
  LibRaw::dcraw_clear_mem(libraw_image);

  gimi::Image foobar(10, 10);
}