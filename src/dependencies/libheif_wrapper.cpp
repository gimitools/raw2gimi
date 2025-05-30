#include "libheif_wrapper.h"
#include <cstring> // memcpy()
#include <iostream>
#include <libheif/heif.h>
#include <libheif/heif_items.h>

using namespace gimi;

// API

void LibheifWrapper::write_to_heif(const RawImage &rawImage, WriteOptions options) {
  string output_filename = options.output_filename;
  heif_context *ctx = heif_context_alloc();
  heif_compression_format compression = extract_compression(options.codec);
  heif_encoder *encoder;
  heif_image *img;
  heif_image_handle *handle;
  heif_chroma chroma = heif_chroma_interleaved_RGB;
  heif_colorspace colorspace = heif_colorspace_RGB;

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
  Band b = bands[0];                              // Assuming the first band is RGB
  memcpy(data, b.m_data.data(), stride * height); // Copy RGB data to image plane

  he(heif_context_get_encoder_for_format(ctx, compression, &encoder));
  he(heif_context_encode_image(ctx, img, encoder, nullptr, &handle));

  // heif_item_id primary_id = heif_image_handle_get_item_id(handle);
  // gimify(ctx, primary_id);

  he(heif_context_write_to_file(ctx, output_filename.c_str()));
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

heif_chroma LibheifWrapper::extract_chroma(WriteOptions options) {

  // WIP

  heif_chroma libheif_chroma = heif_chroma_undefined;
  gimi::Chroma gimi_chroma = options.chroma;

  if (gimi_chroma == gimi::Chroma::rgb) {
  }

  switch (gimi_chroma) {
  case gimi::Chroma::rgb:
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

  return libheif_chroma;
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