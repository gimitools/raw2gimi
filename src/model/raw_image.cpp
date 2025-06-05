#include "raw_image.h"

using namespace gimi;

// Constructor

gimi::RawImage::RawImage(uint32_t w, uint32_t h) {
  m_width = w;
  m_height = h;
}

// Getters

uint32_t RawImage::get_width() const {
  return m_width;
}

uint32_t RawImage::get_height() const {
  return m_height;
}

uint32_t RawImage::get_bit_depth() const {
  return gimi::get_bit_depth(m_pixel_type);
}

uint32_t RawImage::get_band_count() const {
  return 3;
}

Interleave RawImage::get_interleave() const {
  return m_interleave;
}

PixelType RawImage::get_pixel_type() const {
  return m_pixel_type;
}

const vector<Plane> &RawImage::get_planes() const {
  return planes;
}

// API

void RawImage::add_rgb_interleaved_8bit(const vector<uint8_t> &pixel_data) {
  const int band_count = 3;     // RGB
  const int bytes_per_band = 1; // 8-bit per channel
  if (pixel_data.size() != m_width * m_height * band_count * bytes_per_band) {
    cerr << "Error: Data size does not match image dimensions." << endl;
    return;
  }

  Plane plane(pixel_data, m_width, m_height, PixelType::uint8);
  planes.push_back(plane);

  m_interleave = Interleave::interleaved;
  m_chroma = Chroma::rgb;
  m_pixel_type = PixelType::uint8;
}

void RawImage::add_rgb_interleaved_hdr(const vector<uint8_t> &pixel_data, PixelType pixel_type) {
  const int band_count = 3; // RGB
  const int bytes_per_pixel = 2;
  uint64_t expected_pixel_count = m_width * m_height * band_count * bytes_per_pixel;
  if (pixel_data.size() != expected_pixel_count) {
    cerr << "RawImage Error: Data size does not match image dimensions." << endl;
    cerr << "\tExpected size: " << expected_pixel_count << endl;
    cerr << "\tActual size: " << pixel_data.size() << endl;
    exit(1);
  }

  // Pixel Data
  Plane plane(pixel_data, m_width, m_height, pixel_type);
  planes.push_back(plane);

  // Metadata
  m_interleave = Interleave::interleaved;
  m_chroma = Chroma::rgb;
  m_pixel_type = pixel_type;
}

void RawImage::add_rgb_planar_8bit(const vector<uint8_t> &pixels_r,
                                   const vector<uint8_t> &pixels_g,
                                   const vector<uint8_t> &pixels_b) {

  const int bytes_per_pixel = 1;
  uint64_t expected_pixel_count = m_width * m_height * bytes_per_pixel;

  if (pixels_r.size() != expected_pixel_count ||
      pixels_g.size() != expected_pixel_count ||
      pixels_b.size() != expected_pixel_count) {
    cerr << "RawImage Error: Data size does not match image dimensions." << endl;
    cerr << "\tExpected size: " << expected_pixel_count << endl;
    cerr << "\tActual sizes: "
         << "R: " << pixels_r.size()
         << ", G: " << pixels_g.size()
         << ", B: " << pixels_b.size() << endl;
    exit(1);
  }

  // Pixel Data
  Plane plane_r(pixels_r, m_width, m_height, PixelType::uint8);
  Plane plane_g(pixels_g, m_width, m_height, PixelType::uint8);
  Plane plane_b(pixels_b, m_width, m_height, PixelType::uint8);
  planes.push_back(plane_r);
  planes.push_back(plane_g);
  planes.push_back(plane_b);

  // Metadata
  m_interleave = Interleave::planar;
  m_chroma = Chroma::rgb;
  m_pixel_type = PixelType::uint8;
}