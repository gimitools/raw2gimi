#include "raw_image.h"
#include "error_handler.h"

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

Chroma RawImage::get_chroma() const {
  return m_chroma;
}

bool RawImage::is_little_endian() const {
  return little_endian;
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

void RawImage::add_rgb_planar_16bit(const vector<uint8_t> &pixels_r,
                                    const vector<uint8_t> &pixels_g,
                                    const vector<uint8_t> &pixels_b) {
  const int bytes_per_pixel = 2;
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
  Plane plane_r(pixels_r, m_width, m_height, PixelType::uint16);
  Plane plane_g(pixels_g, m_width, m_height, PixelType::uint16);
  Plane plane_b(pixels_b, m_width, m_height, PixelType::uint16);
  planes.push_back(plane_r);
  planes.push_back(plane_g);
  planes.push_back(plane_b);

  // Metadata
  m_interleave = Interleave::planar;
  m_chroma = Chroma::rgb;
  m_pixel_type = PixelType::uint16;
}

void RawImage::add_yuv_444_interleaved_8bit(const vector<uint8_t> &pixels) {
  const int band_count = 3;     // RGB
  const int bytes_per_band = 1; // 8-bit per channel
  const uint64_t expected_pixel_count = m_width * m_height * band_count * bytes_per_band;
  if (pixels.size() != expected_pixel_count) {
    throw_error("Bad vector size: %zu, expected: %d", pixels.size(), expected_pixel_count);
  }

  Plane plane(pixels, m_width, m_height, PixelType::uint8);
  planes.push_back(plane);

  m_interleave = Interleave::interleaved;
  m_chroma = Chroma::yuv_444;
  m_pixel_type = PixelType::uint8;
}

void RawImage::add_yuv_444_planar_8bit(const vector<uint8_t> &pixels_y,
                                       const vector<uint8_t> &pixels_u,
                                       const vector<uint8_t> &pixels_v) {
  const int bytes_per_pixel = 1;
  uint64_t expected_pixel_count = m_width * m_height * bytes_per_pixel;

  if (pixels_y.size() != expected_pixel_count ||
      pixels_u.size() != expected_pixel_count ||
      pixels_v.size() != expected_pixel_count) {
    cerr << "RawImage Error: Data size does not match image dimensions." << endl;
    cerr << "\tExpected size: " << expected_pixel_count << endl;
    cerr << "\tActual sizes: "
         << "R: " << pixels_y.size()
         << ", G: " << pixels_u.size()
         << ", B: " << pixels_v.size() << endl;
    exit(1);
  }

  // Pixel Data
  Plane plane_r(pixels_y, m_width, m_height, PixelType::uint8);
  Plane plane_g(pixels_u, m_width, m_height, PixelType::uint8);
  Plane plane_b(pixels_v, m_width, m_height, PixelType::uint8);
  planes.push_back(plane_r);
  planes.push_back(plane_g);
  planes.push_back(plane_b);

  // Metadata
  m_interleave = Interleave::planar;
  m_chroma = Chroma::yuv_444;
  m_pixel_type = PixelType::uint8;
}

void RawImage::add_yuv_420_planar_8bit(const vector<uint8_t> &y,
                                       const vector<uint8_t> &u,
                                       const vector<uint8_t> &v) {
  auto yuv444 = convert_420_to_444(y, u, v);
  add_yuv_444_planar_8bit(yuv444[0], yuv444[1], yuv444[2]);
}

vector<vector<uint8_t>> RawImage::convert_420_to_444(const vector<uint8_t> &y,
                                                     const vector<uint8_t> &u_420,
                                                     const vector<uint8_t> &v_420) {
  vector<vector<uint8_t>> yuv444;
  yuv444.push_back(y);

  // convert u_420 to u_444
  vector<uint8_t> u_444;
  for (uint32_t row = 0; row < m_height; ++row) {
    for (uint32_t col = 0; col < m_width; ++col) {
      // Find the corresponding pixel in the 420 image
      uint32_t src_row = row / 2;
      uint32_t src_col = col / 2;
      uint32_t src_index = src_row * (m_width / 2) + src_col;
      u_444.push_back(u_420[src_index]);
    }
  }
  yuv444.push_back(u_444);

  // convert v_420 to v_444
  vector<uint8_t> v_444;
  for (uint32_t row = 0; row < m_height; ++row) {
    for (uint32_t col = 0; col < m_width; ++col) {
      // Find the corresponding pixel in the 420 image
      uint32_t src_row = row / 2;
      uint32_t src_col = col / 2;
      uint32_t src_index = src_row * (m_width / 2) + src_col;
      v_444.push_back(v_420[src_index]);
    }
  }
  yuv444.push_back(v_444);

  return yuv444;
}

void RawImage::add_mono_8bit(const vector<uint8_t> &pixels) {
  const int band_count = 1;     // Mono
  const int bytes_per_band = 1; // 8-bit per channel
  const uint64_t expected_pixel_count = m_width * m_height * band_count * bytes_per_band;
  if (pixels.size() != expected_pixel_count) {
    throw_error("Bad vector size: %zu, expected: %d", pixels.size(), expected_pixel_count);
  }

  Plane plane(pixels, m_width, m_height, PixelType::uint8);
  planes.push_back(plane);

  m_chroma = Chroma::gray;
  m_pixel_type = PixelType::uint8;
}

void RawImage::add_mono_16bit(const vector<uint8_t> &pixels) {
  const int band_count = 1;     // Mono
  const int bytes_per_band = 2; // 16-bit per channel
  const uint64_t expected_pixel_count = m_width * m_height * band_count * bytes_per_band;
  if (pixels.size() != expected_pixel_count) {
    throw_error("Bad vector size: %zu, expected: %d", pixels.size(), expected_pixel_count);
  }

  Plane plane(pixels, m_width, m_height, PixelType::uint16);
  planes.push_back(plane);

  m_chroma = Chroma::gray;
  m_pixel_type = PixelType::uint16;
}