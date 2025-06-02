#include "raw_image.h"

using namespace gimi;

gimi::RawImage::RawImage(uint32_t w, uint32_t h) {
  width = w;
  height = h;
}

void gimi::RawImage::add_rgb_interleaved_8bit(const vector<uint8_t> &pixel_data) {
  const int band_count = 3; // RGB
  if (pixel_data.size() != width * height * band_count) {
    cerr << "Error: Data size does not match image dimensions." << endl;
    return;
  }

  Plane plane(pixel_data, width, height, BitDepth::uint8);
  planes.push_back(plane);

  interleave = Interleave::interleaved;
  chroma = Chroma::rgb;
}