#include "file_reader.h"
#include <libraw/libraw.h>

using namespace std;

void FileReader::read_file(string input_filename) {
  gimi::Image image = libraw_decode(input_filename);
}

gimi::Image FileReader::libraw_decode(const string &input_filename) {

  // Open File
  LibRaw libraw;
  re(libraw.open_file(input_filename.c_str()));

  // Decode
  re(libraw.unpack());

  // Bayer Data (TODO)
  uint16_t *bayer_data = libraw.imgdata.rawdata.raw_image;
  int width2 = libraw.imgdata.sizes.raw_width;
  int height2 = libraw.imgdata.sizes.raw_height;
  const char *cdesc = libraw.imgdata.idata.cdesc; // CFA pattern (e.g., "RGGB", "RGBG")

  // Colorize
  re(libraw.dcraw_process());
  int ret;
  libraw_processed_image_t *libraw_image = libraw.dcraw_make_mem_image(&ret);
  re(ret);

  // Convert Image
  gimi::Image image = FileReader::libraw_to_gimi(libraw_image);

  // Free the image memory
  LibRaw::dcraw_clear_mem(libraw_image);

  return image;
}

gimi::Image FileReader::libraw_to_gimi(const libraw_processed_image_t *libraw_image) {

  enum LibRaw_image_formats type = libraw_image->type; // 2 = LIBRAW_IMAGE_BITMAP
  uint32_t width = libraw_image->width;
  uint32_t height = libraw_image->height;
  uint32_t bit_depth = libraw_image->bits;
  uint32_t colors = libraw_image->colors;

  gimi::Image gimi_image(width, height, bit_depth);

  return gimi_image;
}

void FileReader::re(int ret) {
  if (ret != LIBRAW_SUCCESS) {
    cerr << "LibRaw Error: " << libraw_strerror(ret) << endl;
    exit(1);
  }
}
