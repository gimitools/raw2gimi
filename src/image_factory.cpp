#include "image_factory.h"
#include <cstring> //memset()

// Constructor
ImageFactory::ImageFactory(uint32_t width, uint32_t height, heif_chroma chroma, heif_colorspace colorspace, uint32_t bit_depth) {
  m_width = width;
  m_height = height;
  m_chroma = chroma;
  m_colorspace = colorspace;
  m_bit_depth = bit_depth;
}

// Public Functions
heif_image *ImageFactory::create_image_in_memory(string pixel_algorithm) {

  heif_image *img;

  // Colorspace
  // YUV, RGB, or monochrome
  switch (m_colorspace) {
  case heif_colorspace_monochrome:
    img = create_monochrome_image();
    break;
  case heif_colorspace_YCbCr:
    img = create_yuv_image();
    break;
  case heif_colorspace_RGB:
    img = create_rgb_image();
    break;
  default:
    cout << "Unsupported Colorspace: " << m_colorspace << endl;
    exit(1);
  }

  return img;
}

vector<heif_image *> ImageFactory::create_sequence_in_memory(string pixel_algroithm) {
  vector<heif_image *> images;

  for (uint32_t i = 0; i < m_frame_count; i++) {
    heif_image *img = create_image_in_memory(pixel_algroithm);
    images.push_back(img);

    // Shuffle Colors
    uint32_t temp = m_color_1;
    m_color_1 = m_color_2;
    m_color_2 = m_color_3;
    m_color_3 = temp;
  }

  return images;
}

// Protected Functions

void ImageFactory::he(struct heif_error error) {
  // TODO - Don't duplicate this function
  if (error.code) {
    printf("ERROR! - subcode: %d  Message: %s\n", error.subcode, error.message);
    exit(error.code);
  }
}

// Protected Functions

heif_image *ImageFactory::create_yuv_image() {
  heif_image *img;
  he(heif_image_create(m_width, m_height, m_colorspace, m_chroma, &img));

  // Chroma
  switch (m_chroma) {
  case heif_chroma_420:
    goto error;
    break;
  case heif_chroma_422:
    addChannel_yuv_422_8bit(img);
    break;
  case heif_chroma_444:
    addChannel_yuv_444_8bit(img);
    break;
  case heif_chroma_monochrome:
  case heif_chroma_interleaved_RGB:
  case heif_chroma_interleaved_RRGGBB_BE:
    goto error;
    break;
  default:
  error:
    cout << "Error! Colorspace is YUV, but chroma is: " << m_chroma << endl;
    exit(1);
  }

  return img;
}

heif_image *ImageFactory::create_rgb_image() {
  heif_image *img;
  he(heif_image_create(m_width, m_height, m_colorspace, m_chroma, &img));

  // Chroma
  switch (m_chroma) {
  case heif_chroma_monochrome:
    goto error;
    break;
  case heif_chroma_420:
    goto error;
    break;
  case heif_chroma_422:
    goto error;
    break;
  case heif_chroma_444:
    addChannel_rgb_planar_8bit(img);
    break;
  case heif_chroma_interleaved_RGB:
    if (m_bit_depth == 8)
      addChannel_rgb_interleaved_8bit(img);
    break;
  case heif_chroma_interleaved_RRGGBB_BE:
    addChannel_rgb_interleaved_hdr(img);
    break;
  default:
  error:
    cout << "Unsupported Chroma: " << m_chroma << endl;
    exit(1);
  }

  return img;
}

heif_image *ImageFactory::create_monochrome_image() {
  std::cout << "Unsupported Feature!\n";
  exit(1);
}

// Protected Functions

void ImageFactory::addChannel_rgb_interleaved_8bit(heif_image *img) {

  // Add One Interleaved Channel
  heif_channel channel = heif_channel_interleaved;
  he(heif_image_add_plane(img, channel, m_width, m_height, m_bit_depth));

  // Get Interleaved Channel
  int stride;
  uint8_t *data = heif_image_get_plane(img, channel, &stride);

  const uint8_t band_count = 3;
  const uint32_t size = m_height * stride;
  for (int r = 0; r < m_height; r++) {
    for (int c = 0; c < stride; c = c + band_count) {
      uint32_t row_start = r * stride;
      int index = row_start + c;

      data[index + 0] = m_color_1; // R
      data[index + 1] = m_color_2; // G
      data[index + 2] = m_color_3; // B
    }
  }
}

void ImageFactory::addChannel_rgb_planar_8bit(heif_image *img) {
  // Add Channels
  he(heif_image_add_plane(img, heif_channel_R, m_width, m_height, m_bit_depth));
  he(heif_image_add_plane(img, heif_channel_G, m_width, m_height, m_bit_depth));
  he(heif_image_add_plane(img, heif_channel_B, m_width, m_height, m_bit_depth));

  // Get Channels
  int stride_r, stride_g, stride_b;
  uint8_t *data_r = heif_image_get_plane(img, heif_channel_R, &stride_r);
  uint8_t *data_g = heif_image_get_plane(img, heif_channel_G, &stride_g);
  uint8_t *data_b = heif_image_get_plane(img, heif_channel_B, &stride_b);

  uint64_t size = m_height * stride_r;

  memset(data_r, 0x11, size);
  memset(data_g, 0xFF, size);
  memset(data_b, 0x22, size);
}

void ImageFactory::addChannel_rgb_interleaved_hdr(heif_image *img) {

  // Add One Interleaved Channel
  heif_channel channel = heif_channel_interleaved;
  he(heif_image_add_plane(img, channel, m_width, m_height, m_bit_depth));

  // Get Interleaved Channel
  int stride;
  uint8_t *data = heif_image_get_plane(img, channel, &stride);
  uint8_t bytes_per_pixel = m_bit_depth == 8 ? 1 : 2;
  const uint8_t band_count = 3;
  const uint32_t size = m_height * stride * bytes_per_pixel;
  for (int r = 0; r < m_height; r++) {
    for (int c = 0; c < stride; c = c + (band_count * bytes_per_pixel)) {
      uint32_t row_start = r * stride;
      int index = row_start + c;
      data[index + 0] = 0x01; // R
      data[index + 1] = 0x22; // R
      data[index + 2] = 0x0E; // G
      data[index + 3] = 0xFF; // G
      data[index + 4] = 0x03; // B
      data[index + 5] = 0x44; // B
    }
  }
}

void ImageFactory::addChannel_yuv_444_8bit(heif_image *img) {
  // Add Channels
  he(heif_image_add_plane(img, heif_channel_Y, m_width, m_height, m_bit_depth));
  he(heif_image_add_plane(img, heif_channel_Cb, m_width, m_height, m_bit_depth));
  he(heif_image_add_plane(img, heif_channel_Cr, m_width, m_height, m_bit_depth));

  // Get Channels
  int stride_y, stride_cb, stride_cr;
  uint8_t *data_y = heif_image_get_plane(img, heif_channel_Y, &stride_y);
  uint8_t *data_cb = heif_image_get_plane(img, heif_channel_Cb, &stride_cb);
  uint8_t *data_cr = heif_image_get_plane(img, heif_channel_Cr, &stride_cr);

  uint64_t size = m_height * stride_y;

  memset(data_y, 0x11, size);
  memset(data_cb, 0x22, size);
  memset(data_cr, 0x33, size);
}

void ImageFactory::addChannel_yuv_422_8bit(heif_image *img) {
  // Add Channels
  he(heif_image_add_plane(img, heif_channel_Y, m_width, m_height, m_bit_depth));
  he(heif_image_add_plane(img, heif_channel_Cb, m_width, m_height, m_bit_depth));
  he(heif_image_add_plane(img, heif_channel_Cr, m_width, m_height, m_bit_depth));

  // Get Channels
  // WARNING: the strides should be different.
  int stride_y, stride_cb, stride_cr;
  uint8_t *data_y = heif_image_get_plane(img, heif_channel_Y, &stride_y);
  uint8_t *data_cb = heif_image_get_plane(img, heif_channel_Cb, &stride_cb);
  uint8_t *data_cr = heif_image_get_plane(img, heif_channel_Cr, &stride_cr);

  if (stride_y == stride_cb || stride_y == stride_cr) {
    cout << "Error! YUV 422 requires different strides for Y, & Cb/Cr." << endl;
    exit(1);
  }

  uint64_t size_y = m_height * stride_y;
  uint64_t size_cb = m_height * stride_cb;
  uint64_t size_cr = m_height * stride_cr;
  memset(data_y, 0x11, size_y);
  memset(data_cb, 0x22, size_cb);
  memset(data_cr, 0x33, size_cr);
}