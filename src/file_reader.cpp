#include "file_reader.h"
#include <libraw/libraw.h>

using namespace std;

void FileReader::read_file(string input_filename) {
  cout << "Decoding file: " << input_filename << endl;

  LibRaw rawProcessor;
  int ret;

  // Open the RAW file
  ret = rawProcessor.open_file(input_filename.c_str());
  if (ret != LIBRAW_SUCCESS) {
    cerr << "Failed to open file: " << libraw_strerror(ret) << endl;
    return;
  }

  // Unpack the RAW data
  ret = rawProcessor.unpack();
  if (ret != LIBRAW_SUCCESS) {
    cerr << "Failed to unpack file: " << libraw_strerror(ret) << endl;
    return;
  }

  // Process the RAW data (demosaic, gamma correction, etc.)
  ret = rawProcessor.dcraw_process();
  if (ret != LIBRAW_SUCCESS) {
    cerr << "Failed to process RAW data: " << libraw_strerror(ret) << endl;
    return;
  }

  // Get the processed image data
  libraw_processed_image_t *image = rawProcessor.dcraw_make_mem_image(&ret);
  if (!image || ret != LIBRAW_SUCCESS) {
    cerr << "Failed to get processed image: " << libraw_strerror(ret) << endl;
    return;
  }

  cout << "Image decoded: " << image->width << "x" << image->height
       << ", colors: " << image->colors << ", bits: " << image->bits << endl;

  // Free the image memory
  LibRaw::dcraw_clear_mem(image);
}