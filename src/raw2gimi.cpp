#include "raw2gimi.h"
#include "image_factory.h"

// CLI API
void Raw2Gimi::write_image_from_memory(MainArgs args) {
  uint32_t width = args.extract_width();
  uint32_t height = args.extract_height();
  heif_compression_format compression = args.extract_compression();
  heif_colorspace colorspace = args.extract_colorspace();
  heif_chroma chroma = args.extract_chroma();
  int bit_depth = args.extract_bit_depth();
  string pixel_algorithm = args.pixel_algorithm;

  heif_context *ctx = encode_image_from_memory(compression, width, height, chroma, colorspace, bit_depth);

  // WRITE
  he(heif_context_write_to_file(ctx, args.output_filename.c_str()));
  cout << "Created: " << args.output_filename << endl;
}

// Primary Functions
heif_context *Raw2Gimi::encode_image_from_memory(heif_compression_format codec, uint32_t width, uint32_t height, heif_chroma chroma, heif_colorspace colorspace, uint32_t bit_depth) {
  // Create Image
  ImageFactory image_factory(width, height, chroma, colorspace, bit_depth);
  heif_image *img = image_factory.create_image_in_memory("solid");

  // ENCODE
  heif_context *ctx = heif_context_alloc();
  heif_encoder *encoder;
  heif_image_handle *handle;
  he(heif_context_get_encoder_for_format(ctx, codec, &encoder));
  heif_context_encode_image(ctx, img, encoder, nullptr, &handle);

  return ctx;
}

// Helper Functions

void Raw2Gimi::he(struct heif_error error) {
  if (error.code) {
    printf("ERROR! - subcode: %d  Message: %s\n", error.subcode, error.message);
    exit(error.code);
  }
}
