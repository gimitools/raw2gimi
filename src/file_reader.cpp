#include "file_reader.h"
#include "error_handler.h"
#include <libraw/libraw.h>

using namespace std;

gimi::RawImage FileReader::read_file(string input_filename) {
  gimi::RawImage image = libraw_decode(input_filename);
  return image;
}

gimi::RawImage FileReader::libraw_decode(const string &input_filename) {

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

  // Convert RawImage
  gimi::RawImage image = FileReader::libraw_to_gimi(libraw_image);

  // Free the image memory
  LibRaw::dcraw_clear_mem(libraw_image);

  return image;
}

gimi::RawImage FileReader::libraw_to_gimi(const libraw_processed_image_t *libraw_image) {

  enum LibRaw_image_formats type = libraw_image->type; // 2 = LIBRAW_IMAGE_BITMAP
  uint32_t width = libraw_image->width;
  uint32_t height = libraw_image->height;
  uint32_t colors = libraw_image->colors;
  uint32_t bit_depth = libraw_image->bits;

  if (colors != 3) {
    cout << "TODO: Handle non-RGB images (colors != 3)" << endl;
    exit(1);
  } else if (bit_depth != 8) {
    cout << "TODO: Handle non-8-bit images (bit_depth != 8)" << endl;
    exit(1);
  } else if (type != LIBRAW_IMAGE_BITMAP) {
    cout << "TODO: Handle non-BITMAP images (type != LIBRAW_IMAGE_BITMAP)" << endl;
    exit(1);
  }

  libraw_image->data_size;
  libraw_image->data;

  // gimi::RawImage gimi_image(width, height, bit_depth);
  gimi::RawImage gimi_image(width, height);

  vector<uint8_t> pixels(libraw_image->data, libraw_image->data + libraw_image->data_size);

  // gimi_image.add_rgb_interleaved_8bit_band(pixels);

  return gimi_image;
}

void FileReader::re(int ret) {
  if (ret != LIBRAW_SUCCESS) {
    throw_error("");
  }
}
