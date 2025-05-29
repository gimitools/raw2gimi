#include "gimifier.h"
#include <cstring> // memcpy()
#include <libheif/heif.h>
#include <libheif/heif_items.h>

using namespace gimi;

void he(struct heif_error error) {
  if (error.code) {
    printf("ERROR! - subcode: %d  Message: %s\n", error.subcode, error.message);
    exit(error.code);
  }
}

void Gimifier::write_to_file(const Image &image, const string &output_filename) {
  libheif_write_to_heif(image, output_filename);
}

void Gimifier::libheif_write_to_heif(const Image &image, const string &output_filename) {

  heif_context *ctx = heif_context_alloc();
  heif_compression_format compression = heif_compression_HEVC;
  heif_encoder *encoder;
  heif_image *img;
  heif_image_handle *handle;
  heif_chroma chroma = heif_chroma_interleaved_RGB;
  heif_colorspace colorspace = heif_colorspace_RGB;

  uint32_t width = image.get_width();
  uint32_t height = image.get_height();
  uint32_t bit_depth = image.get_bit_depth();

  heif_channel channel = heif_channel_interleaved;
  he(heif_image_create(width, height, colorspace, chroma, &img));
  he(heif_image_add_plane(img, channel, width, height, bit_depth));

  // Get Channels
  int stride;
  uint8_t *data = heif_image_get_plane(img, channel, &stride);

  const vector<Band> bands = image.get_bands();
  Band b = bands[0];                              // Assuming the first band is RGB
  memcpy(data, b.m_data.data(), stride * height); // Copy RGB data to image plane

  he(heif_context_get_encoder_for_format(ctx, compression, &encoder));
  he(heif_context_encode_image(ctx, img, encoder, nullptr, &handle));

  // heif_item_id primary_id = heif_image_handle_get_item_id(handle);
  // gimify(ctx, primary_id);

  he(heif_context_write_to_file(ctx, output_filename.c_str()));
  printf("Created: %s\n", output_filename.c_str());
}
