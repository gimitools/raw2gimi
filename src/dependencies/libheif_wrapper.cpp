#include "libheif_wrapper.h"
#include "error_handler.h"
#include <cstdint>
#include <cstdlib>
#include <cstring> // memcpy()
#include <iomanip>
#include <iostream>
#include <libheif/heif.h>
#include <libheif/heif_items.h>
#include <libheif/heif_properties.h>
#include <libheif/heif_sequences.h>
#include <libheif/heif_tai_timestamps.h>
#include <sstream>
#include <string>
using namespace gimi;

// Constructor

LibheifWrapper::LibheifWrapper(WriteOptions options) {
  m_ctx = heif_context_alloc();
  m_options = options;
}

LibheifWrapper::~LibheifWrapper() {
  if (m_ctx) {
    heif_context_free(m_ctx);
    m_ctx = nullptr;
  }
}

// API

void LibheifWrapper::add_image(const RawImage &rawImage) {
  heif_image *img;
  heif_image_handle *handle;
  heif_encoder *encoder;
  heif_compression_format compression = extract_compression(m_options.codec);
  heif_chroma chroma = extract_chroma(rawImage);
  heif_colorspace colorspace = extract_colorspace(rawImage);

  img = convert_to_heif_image(rawImage, colorspace, chroma);

  he(heif_context_get_encoder_for_format(m_ctx, compression, &encoder));
  he(heif_context_encode_image(m_ctx, img, encoder, nullptr, &handle));

  heif_item_id primary_id = heif_image_handle_get_item_id(handle);
  gimify(primary_id);
}

void LibheifWrapper::add_grid(const RawImageGrid &rawImages) {
  vector<heif_image *> tiles;
  heif_image *img;
  heif_image_handle *handle;
  heif_encoder *encoder;
  heif_compression_format compression = extract_compression(m_options.codec);
  heif_chroma chroma = extract_chroma(rawImages[0][0]);
  heif_colorspace colorspace = extract_colorspace(rawImages[0][0]);

  // Convert Tiles
  for (const vector<RawImage> &rows : rawImages) {
    for (const RawImage &tile : rows) {
      img = convert_to_heif_image(tile, colorspace, chroma);
      tiles.push_back(img);
    }
  }

  // Get Encoder
  he(heif_context_get_encoder_for_format(m_ctx, compression, &encoder));

  uint32_t tile_rows = rawImages.size();
  uint32_t tile_columns = rawImages[0].size();
  uint32_t tile_width = rawImages[0][0].get_width();
  uint32_t tile_height = rawImages[0][0].get_height();
  uint32_t image_width = tile_width * tile_columns;
  uint32_t image_height = tile_height * tile_rows;
  heif_encoding_options *encoding_options = heif_encoding_options_alloc();
  he(heif_context_add_grid_image(
      m_ctx,
      image_width,
      image_height,
      tile_columns,
      tile_rows,
      encoding_options,
      &handle));

  heif_item_id primary_id = heif_image_handle_get_item_id(handle);
  heif_context_set_primary_image(m_ctx, handle);

  for (uint32_t row = 0; row < tile_rows; row++) {
    for (uint32_t column = 0; column < tile_columns; column++) {
      uint32_t index = row * tile_columns + column;
      he(heif_context_add_image_tile(
          m_ctx,
          handle,
          column,
          row,
          tiles[index],
          encoder));

      heif_item_id out_tile_id;
      bool process_image_transformations = 0;
      heif_image_handle_get_grid_image_tile_id(
          handle,
          process_image_transformations,
          column,
          row,
          &out_tile_id);
      add_content_id(out_tile_id);
    }
  }

  gimify(primary_id);

  heif_encoding_options_free(encoding_options);
}

void LibheifWrapper::add_video(const vector<RawImage> &rawImages) {

  // Variables
  struct heif_track_options *track_options = heif_track_options_alloc();
  struct heif_sequence_encoding_options *seq_options = heif_sequence_encoding_options_alloc();
  struct heif_tai_clock_info *clock_info = heif_tai_clock_info_alloc();
  heif_track *track;
  uint16_t width = rawImages[0].get_width();
  uint16_t height = rawImages[0].get_height();
  heif_image *img;
  heif_chroma chroma = extract_chroma(rawImages[0]);
  heif_colorspace colorspace = extract_colorspace(rawImages[0]);

  // Track Content ID
  heif_track_options_set_gimi_track_id(track_options, generate_content_id().c_str());

  // Require GIMI metadata
  heif_sample_aux_info_presence mandatory = heif_sample_aux_info_presence_mandatory;
  heif_track_options_enable_sample_gimi_content_ids(track_options, mandatory);
  heif_track_options_enable_sample_tai_timestamps(track_options, clock_info, mandatory);

  // Add Video Track
  he(heif_context_add_visual_sequence_track(
      m_ctx,
      width, height,
      heif_track_type_video,
      track_options,
      seq_options,
      &track));

  // Get Encoder
  heif_compression_format compression = extract_compression(m_options.codec);
  heif_encoder *encoder;
  he(heif_context_get_encoder_for_format(m_ctx, compression, &encoder));

  // Encode Samples
  uint32_t duration = 90000;
  for (const RawImage &rawImage : rawImages) {
    img = convert_to_heif_image(rawImage, colorspace, chroma);
    heif_image_set_duration(img, duration);

    // Sample Content ID
    heif_image_set_gimi_sample_content_id(img, generate_content_id().c_str());

    // Timestamp
    heif_tai_timestamp_packet *timestamp = heif_tai_timestamp_packet_alloc();
    heif_image_set_tai_timestamp(img, timestamp);

    // Encode Sample
    he(heif_track_encode_sequence_image(
        track,
        img,
        encoder,
        seq_options));

    heif_tai_timestamp_packet_release(timestamp);
  }

  // Cleanup
  heif_tai_clock_info_release(clock_info);
  heif_track_options_release(track_options);
  heif_sequence_encoding_options_release(seq_options);
}

void LibheifWrapper::add_metadata_track() {
  struct heif_track_options *options = nullptr;
  const char *uri = "dummy_uri_for_metadata_track";
  heif_track *track;
  he(heif_context_add_uri_metadata_sequence_track(
      m_ctx,
      uri,
      options,
      &track));

  heif_raw_sequence_sample *raw_sequence_sample = heif_raw_sequence_sample_alloc();

  // Set Data
  const uint8_t *data;
  string dummy_data = "dummy data for sequence sample";
  size_t size = dummy_data.size();

  // Add Samples
  for (uint32_t i = 0; i < 10; i++) {
    heif_raw_sequence_sample_set_gimi_sample_content_id(raw_sequence_sample, generate_content_id().c_str());
    he(heif_raw_sequence_sample_set_data(raw_sequence_sample, (uint8_t *)dummy_data.data(), size));
    heif_raw_sequence_sample_set_duration(raw_sequence_sample, 90000);
    he(heif_track_add_raw_sequence_sample(track, raw_sequence_sample));
  }
}

void LibheifWrapper::write_to_heif() {
  string output_filename = m_options.output_filename;
  he(heif_context_write_to_file(m_ctx, output_filename.c_str()));
}

// GIMI

void LibheifWrapper::gimify(heif_item_id primary_id) {
  heif_context_add_compatible_brand(m_ctx, heif_fourcc('g', 'e', 'o', '1'));
  heif_context_add_compatible_brand(m_ctx, heif_fourcc('u', 'n', 'i', 'f'));

  // Content Id
  add_content_id(primary_id);

  // Timestamp
  add_timestamp(primary_id);
}

void LibheifWrapper::add_content_id(heif_item_id id) {
  // const char *extended_type_content_id = "0x261ef3741d975bbaacbd9d2c8ea73522";
  uint8_t extended_type_content_id[16] = {
      0x26, 0x1e, 0xf3, 0x74, 0x1d, 0x97, 0x5b, 0xba,
      0xac, 0xbd, 0x9d, 0x2c, 0x8e, 0xa7, 0x35, 0x22};
  string content_id = generate_content_id();

  // Content Id
  he(heif_item_add_raw_property(
      m_ctx,
      id,
      heif_fourcc('u', 'u', 'i', 'd'),
      (const uint8_t *)extended_type_content_id,
      (const uint8_t *)content_id.c_str(),
      content_id.length(),
      0, // is_essential - not essential for viewing
      nullptr));
}

void LibheifWrapper::add_timestamp(heif_item_id id) {
  heif_tai_clock_info *clock = heif_tai_clock_info_alloc();
  heif_tai_timestamp_packet *timestamp = heif_tai_timestamp_packet_alloc();

  heif_item_set_property_tai_clock_info(m_ctx, id, clock, nullptr);
  heif_item_set_property_tai_timestamp(m_ctx, id, timestamp, nullptr);

  heif_tai_clock_info_release(clock);
  heif_tai_timestamp_packet_release(timestamp);
}

// Helper Functions

heif_image *LibheifWrapper::convert_to_heif_image(const RawImage &rawImage, heif_colorspace colorspace, heif_chroma chroma) {
  heif_image *img;
  switch (colorspace) {
  case heif_colorspace_YCbCr:
    img = convert_yuv_colorspace(rawImage, chroma);
    break;
  case heif_colorspace_RGB:
    img = convert_rgb_colorspace(rawImage, chroma);
    break;
  case heif_colorspace_monochrome:
    img = convert_gray(rawImage);
    break;
  default:
    throw_error("Unsupported colorspace: %d", static_cast<int>(colorspace));
  }
  return img;
}

heif_image *LibheifWrapper::convert_yuv_colorspace(const RawImage &rawImage, heif_chroma chroma) {

  // Assert Planar
  gimi::Interleave interleave = rawImage.get_interleave();
  if (interleave != gimi::Interleave::planar) {
    throw_error("libheif doesn't support yuv interleaved");
  }

  switch (chroma) {
  case heif_chroma_444:
    return convert_yuv_444_planar_8bit(rawImage);
    break;
  case heif_chroma_422:
    break;
  case heif_chroma_420:
    break;
  default:
    throw_error("Expected yuv chroma but got: %d", chroma);
  }

  throw_error("Unhandled chroma: %d", chroma);
  return nullptr;
}

heif_image *LibheifWrapper::convert_rgb_colorspace(const RawImage &rawImage, heif_chroma chroma) {
  switch (chroma) {
  case heif_chroma_444: // planar
    return convert_rgb_planar(rawImage);
  case heif_chroma_interleaved_RGB:
    return convert_rgb_interleaved(rawImage);
  case heif_chroma_interleaved_RGBA:
    break;
  case heif_chroma_interleaved_RRGGBB_BE:
    return convert_rgb_interleaved_hdr_be(rawImage);
    break;
  case heif_chroma_interleaved_RRGGBBAA_BE:
    break;
  case heif_chroma_interleaved_RRGGBB_LE:
    break;
  case heif_chroma_interleaved_RRGGBBAA_LE:
    break;
  default:
    throw_error("Unsupported chroma format: %d", static_cast<int>(chroma));
  }
  return nullptr;
}

// Leaf Functions

heif_image *LibheifWrapper::convert_gray(const RawImage &rawImage) {

  gimi::PixelType pixel_type = rawImage.get_pixel_type();

  heif_image *img;

  heif_colorspace colorspace = heif_colorspace_monochrome;
  heif_channel channel = heif_channel_Y;
  heif_chroma chroma = heif_chroma_monochrome;

  uint32_t width = rawImage.get_width();
  uint32_t height = rawImage.get_height();
  uint32_t bit_depth = rawImage.get_bit_depth();

  he(heif_image_create(width, height, colorspace, chroma, &img));
  he(heif_image_add_plane(img, channel, width, height, bit_depth));

  // Get Channels
  int stride;
  uint8_t *data = heif_image_get_plane(img, channel, &stride);

  const vector<Plane> &planes = rawImage.get_planes();
  if (planes.size() != 1) {
    throw_error("Expected 1 planes, but got: %d", planes.size());
  }

  const Plane &plane = planes[0];
  const vector<uint8_t> &plane_y = planes[0].m_pixels;

  if (plane_y.size() != stride * height) {
    throw_error("Expected size: %d, but got: %zu", stride * height, plane_y.size());
  }

  memcpy(data, plane_y.data(), stride * height);

  return img;
}

heif_image *LibheifWrapper::convert_rgb_planar(const RawImage &rawImage) {
  heif_image *img;

  heif_colorspace colorspace = heif_colorspace_RGB;
  heif_channel channel1 = heif_channel_R;
  heif_channel channel2 = heif_channel_G;
  heif_channel channel3 = heif_channel_B;
  heif_chroma chroma = heif_chroma_444;

  uint32_t width = rawImage.get_width();
  uint32_t height = rawImage.get_height();
  uint32_t bit_depth = rawImage.get_bit_depth();

  he(heif_image_create(width, height, colorspace, chroma, &img));
  he(heif_image_add_plane(img, channel1, width, height, bit_depth));
  he(heif_image_add_plane(img, channel2, width, height, bit_depth));
  he(heif_image_add_plane(img, channel3, width, height, bit_depth));

  // Get Channels
  int stride;
  uint8_t *data1 = heif_image_get_plane(img, channel1, &stride);
  uint8_t *data2 = heif_image_get_plane(img, channel2, &stride);
  uint8_t *data3 = heif_image_get_plane(img, channel3, &stride);

  const vector<Plane> &planes = rawImage.get_planes();
  if (planes.size() != 3) {
    throw_error("Expected 3 planes, but got: %d", planes.size());
  }

  const Plane &plane = planes[0];
  const vector<uint8_t> &plane_r = planes[0].m_pixels;
  const vector<uint8_t> &plane_g = planes[1].m_pixels;
  const vector<uint8_t> &plane_b = planes[2].m_pixels;

  if (plane_r.size() != stride * height) {
    throw_error("Expected size: %d, but got: %zu", stride * height, plane_r.size());
  }

  memcpy(data1, plane_r.data(), stride * height);
  memcpy(data2, plane_g.data(), stride * height);
  memcpy(data3, plane_b.data(), stride * height);

  return img;
}

heif_image *LibheifWrapper::convert_rgb_interleaved(const RawImage &rawImage) {
  heif_image *img;

  heif_colorspace colorspace = heif_colorspace_RGB;
  heif_channel channel = heif_channel_interleaved;
  heif_chroma chroma = heif_chroma_interleaved_RGB;

  uint32_t width = rawImage.get_width();
  uint32_t height = rawImage.get_height();
  uint32_t bit_depth = rawImage.get_bit_depth();

  he(heif_image_create(width, height, colorspace, chroma, &img));
  he(heif_image_add_plane(img, channel, width, height, bit_depth));

  // Get Channels
  int stride;
  uint8_t *data = heif_image_get_plane(img, channel, &stride);

  const vector<Plane> &planes = rawImage.get_planes();
  if (planes.size() != 1) {
    throw_error("Expected 1 plane, but got: %d", planes.size());
  }
  const Plane &plane = planes[0];
  const vector<uint8_t> &pixels = plane.m_pixels;
  if (pixels.size() != stride * height) {
    throw_error("Expected size: %d, but got: %zu", stride * height, pixels.size());
  }

  memcpy(data, pixels.data(), stride * height); // Copy RGB data to image plane

  return img;
}

heif_image *LibheifWrapper::convert_rgb_interleaved_hdr_be(const RawImage &rawImage) {
  heif_image *img;

  heif_colorspace colorspace = heif_colorspace_RGB;
  heif_channel channel = heif_channel_interleaved;
  heif_chroma chroma = heif_chroma_interleaved_RRGGBB_BE;

  uint32_t width = rawImage.get_width();
  uint32_t height = rawImage.get_height();
  uint32_t bit_depth = rawImage.get_bit_depth();

  he(heif_image_create(width, height, colorspace, chroma, &img));
  he(heif_image_add_plane(img, channel, width, height, bit_depth));

  // Get Channels
  int stride;
  uint8_t *data = heif_image_get_plane(img, channel, &stride);

  const vector<Plane> &planes = rawImage.get_planes();
  if (planes.size() != 1) {
    throw_error("Expected 1 plane, but got: %d", planes.size());
  }
  const Plane &plane = planes[0];
  const vector<uint8_t> &pixels = plane.m_pixels;
  if (pixels.size() != stride * height) {
    throw_error("Expected size: %d, but got: %zu", stride * height, pixels.size());
  }

  memcpy(data, pixels.data(), stride * height); // Copy RGB data to image plane

  return img;
}

heif_image *LibheifWrapper::convert_yuv_444_planar_8bit(const RawImage &rawImage) {
  heif_image *img;

  heif_colorspace colorspace = heif_colorspace_YCbCr;
  heif_channel channel1 = heif_channel_Y;
  heif_channel channel2 = heif_channel_Cb;
  heif_channel channel3 = heif_channel_Cr;
  heif_chroma chroma = heif_chroma_444;

  uint32_t width = rawImage.get_width();
  uint32_t height = rawImage.get_height();
  uint32_t bit_depth = rawImage.get_bit_depth();

  he(heif_image_create(width, height, colorspace, chroma, &img));
  he(heif_image_add_plane(img, channel1, width, height, bit_depth));
  he(heif_image_add_plane(img, channel2, width, height, bit_depth));
  he(heif_image_add_plane(img, channel3, width, height, bit_depth));

  // Get Channels
  int stride;
  uint8_t *data1 = heif_image_get_plane(img, channel1, &stride);
  uint8_t *data2 = heif_image_get_plane(img, channel2, &stride);
  uint8_t *data3 = heif_image_get_plane(img, channel3, &stride);

  const vector<Plane> &planes = rawImage.get_planes();
  if (planes.size() != 3) {
    throw_error("Expected 3 planes, but got: %d", planes.size());
  }

  const Plane &plane = planes[0];
  const vector<uint8_t> &plane_y = planes[0].m_pixels;
  const vector<uint8_t> &plane_u = planes[1].m_pixels;
  const vector<uint8_t> &plane_v = planes[2].m_pixels;

  if (plane_y.size() != stride * height) {
    throw_error("Expected size: %d, but got: %zu", stride * height, plane_y.size());
  }

  memcpy(data1, plane_y.data(), stride * height);
  memcpy(data2, plane_u.data(), stride * height);
  memcpy(data3, plane_v.data(), stride * height);

  return img;
}

// Static Functions

void LibheifWrapper::he(struct heif_error error) {
  if (error.code) {
    printf("libheif wrapper error!!\n");
    printf("ERROR! - subcode: %d  Message: %s\n", error.subcode, error.message);
    exit(error.code);
  }
}

heif_compression_format LibheifWrapper::extract_compression(gimi::Codec codec) {
  switch (codec) {
  case Codec::raw:
    return heif_compression_uncompressed;
  case Codec::hevc:
    return heif_compression_HEVC;
  case Codec::avc:
    return heif_compression_AVC;
  case Codec::vvc:
    return heif_compression_VVC;
  case Codec::av1:
    return heif_compression_AV1;
  case Codec::jpeg:
    return heif_compression_JPEG;
  case Codec::j2k:
    return heif_compression_JPEG2000;
  case Codec::htj2k:
    return heif_compression_HTJ2K;
  }
  return heif_compression_undefined;
}

heif_colorspace LibheifWrapper::extract_colorspace(const RawImage &rawImage) {
  Chroma gimi_chroma = rawImage.get_chroma();
  switch (gimi_chroma) {
  case gimi::Chroma::rgb:
    return heif_colorspace_RGB;
  case gimi::Chroma::gray:
    return heif_colorspace_monochrome;
  case gimi::Chroma::yuv_444:
  case gimi::Chroma::yuv_422:
  case gimi::Chroma::yuv_420:
    return heif_colorspace_YCbCr;
  default:
    throw_error("Unsupported chroma format: %d", (int)gimi_chroma);
  }

  return heif_colorspace_undefined;
}

heif_chroma LibheifWrapper::extract_chroma(const RawImage &image) {

  uint32_t nbands = image.get_band_count();
  uint32_t bit_depth = image.get_bit_depth();

  gimi::Chroma gimi_chroma = image.get_chroma();
  gimi::Interleave gimi_interleave = image.get_interleave();
  bool little_endian = image.is_little_endian();
  bool big_endian = !little_endian;

  switch (gimi_chroma) {
  case gimi::Chroma::rgb:
    if (gimi_interleave == gimi::Interleave::planar) {
      return heif_chroma_444; // Planar RGB
    } else if (bit_depth == 8 && nbands == 3) {
      return heif_chroma_interleaved_RGB;
    } else if (bit_depth == 8 && nbands == 4) {
      return heif_chroma_interleaved_RGBA;
    } else if (bit_depth > 8 && nbands == 3 && big_endian) {
      return heif_chroma_interleaved_RRGGBB_BE;
    } else if (bit_depth > 8 && nbands == 4 && big_endian) {
      return heif_chroma_interleaved_RRGGBBAA_BE;
    } else if (bit_depth > 8 && nbands == 3 && little_endian) {
      return heif_chroma_interleaved_RRGGBB_LE;
    } else if (bit_depth > 8 && nbands == 4 && little_endian) {
      return heif_chroma_interleaved_RRGGBBAA_LE;
    }
    break;
  case gimi::Chroma::gray:
    return heif_chroma_monochrome;
  case gimi::Chroma::yuv_444:
    return heif_chroma_444;
  case gimi::Chroma::yuv_422:
    return heif_chroma_422;
  case gimi::Chroma::yuv_420:
    return heif_chroma_420;
  case gimi::Chroma::yuv_411:
    throw_error("Unsupported chroma format: yuv_411");
  }

  throw_error("Unsupported chroma format: %d", (int)gimi_chroma);
  return heif_chroma_undefined;
}

string LibheifWrapper::generate_content_id() {
  uint64_t part5 = (static_cast<uint64_t>(rand()) << 32) | rand();

  std::ostringstream oss;
  oss << "urn:uuid:"
      << std::hex << std::setfill('0')
      << std::setw(8) << rand() << "-"
      << std::setw(4) << (rand() % 0x10000) << "-"
      << std::setw(4) << (rand() % 0x10000) << "-"
      << std::setw(4) << (rand() % 0x10000) << "-"
      << std::setw(12) << part5;

  return oss.str();
}