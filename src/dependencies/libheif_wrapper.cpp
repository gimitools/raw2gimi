#include "libheif_wrapper.h"
#include <cstring> // memcpy()
#include <libheif/heif.h>
#include <libheif/heif_items.h>

using namespace gimi;

// API

void LibheifWrapper::write_to_heif(const RawImage &rawImage, gimi::Codec codec, const string &output_filename) {

  heif_context *ctx = heif_context_alloc();
  heif_compression_format compression = get_compression_format(codec);
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

heif_compression_format LibheifWrapper::get_compression_format(gimi::Codec codec) {
  return heif_compression_format();
}
// heif_compression_format LibheifWrapper::get_compression_format(gimi::Codec codec) {
// }
