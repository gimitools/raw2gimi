#include "raw_image.h"

using namespace gimi;

gimi::RawImage::RawImage(uint32_t w, uint32_t h) {
  width = w;
  height = h;
}

void gimi::RawImage::add_rgb_interleaved_8bit(const vector<uint8_t> &pixel_data) {
  const int band_count = 3;     // RGB
  const int bytes_per_band = 1; // 8-bit per channel
  if (pixel_data.size() != width * height * band_count * bytes_per_band) {
    cerr << "Error: Data size does not match image dimensions." << endl;
    return;
  }

  Plane plane(pixel_data, width, height, PixelType::uint8);
  planes.push_back(plane);

  interleave = Interleave::interleaved;
  chroma = Chroma::rgb;
  pixel_type = PixelType::uint8;
}

void gimi::RawImage::add_rgb_interleaved_hdr(const vector<uint16_t> &pixel_data, PixelType pixel_type) {
  const int band_count = 3;     // RGB
  const int bytes_per_band = 2; // 16-bit per channel
  if (pixel_data.size() != width * height * band_count * bytes_per_band) {
    cerr << "Error: Data size does not match image dimensions." << endl;
    return;
  }

  Plane plane(pixel_data, width, height, PixelType::uint16);
  planes.push_back(plane);

  interleave = Interleave::interleaved;
  chroma = Chroma::rgb;
  pixel_type = PixelType::uint16;
}