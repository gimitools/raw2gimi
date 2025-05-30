#include "libheif_wrapper.h"
#include <cstring> // memcpy()
#include <iostream>
#include <libheif/heif.h>
#include <libheif/heif_items.h>

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
  // heif_context *m_ctx = heif_context_alloc();
  heif_compression_format compression = extract_compression(m_options.codec);
  heif_encoder *encoder;
  heif_image *img;
  heif_image_handle *handle;
  heif_chroma chroma = extract_chroma(rawImage, m_options);
  heif_colorspace colorspace = extract_colorspace(m_options.chroma, m_options.interleave);

  uint32_t width = rawImage.get_width();
  uint32_t height = rawImage.get_height();
  uint32_t bit_depth = rawImage.get_bit_depth();

  heif_channel channel = heif_channel_interleaved;
  he(heif_image_create(width, height, colorspace, chroma, &img));
  he(heif_image_add_plane(img, channel, width, height, bit_depth));

  // Get Channels
  int stride;
  uint8_t *data = heif_image_get_plane(img, channel, &stride);

  const vector<Band> bands = rawImage.get_bands();

  // TODO: dont assume the first band is RGB
  Band b = bands[0];
  memcpy(data, b.m_data.data(), stride * height); // Copy RGB data to image plane

  he(heif_context_get_encoder_for_format(m_ctx, compression, &encoder));
  he(heif_context_encode_image(m_ctx, img, encoder, nullptr, &handle));

  // heif_item_id primary_id = heif_image_handle_get_item_id(handle);
  // gimify(m_ctx, primary_id);
}

void LibheifWrapper::write_to_heif() {
  string output_filename = m_options.output_filename;
  he(heif_context_write_to_file(m_ctx, output_filename.c_str()));
  printf("Created: %s\n", output_filename.c_str());
}

// Helper Functions

void LibheifWrapper::he(struct heif_error error) {
  if (error.code) {
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

heif_colorspace LibheifWrapper::extract_colorspace(Chroma gimi_chroma, Interleave gimi_interleave) {
  switch (gimi_chroma) {
  case gimi::Chroma::rgb:
    return heif_colorspace_RGB;
  case gimi::Chroma::gray:
    return heif_colorspace_monochrome;
  case gimi::Chroma::yuv_444:
    if (gimi_interleave == Interleave::planar) {
      return heif_colorspace_YCbCr;
    } else {
      return heif_colorspace_RGB;
    }
  case gimi::Chroma::yuv_422:
  case gimi::Chroma::yuv_420:
    return heif_colorspace_YCbCr;
  default:
    cerr << "libheif_wrapper::extract_colorspace(): Unsupported chroma format: " << static_cast<int>(gimi_chroma) << endl;
    exit(1);
  }

  return heif_colorspace_undefined;
}

heif_chroma LibheifWrapper::extract_chroma(const RawImage &image, WriteOptions options) {

  uint32_t nbands = image.get_band_count();
  uint32_t bit_depth = image.get_bit_depth();

  gimi::Chroma gimi_chroma = options.chroma;
  gimi::Interleave gimi_interleave = options.interleave;
  bool little_endian = options.little_endian;
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
    cerr << "libheif_wrapper::extract_chroma(): Unsupported chroma format: yuv_411" << endl;
    exit(1);
  }

  return heif_chroma_undefined;
}
