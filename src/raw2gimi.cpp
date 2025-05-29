#include "raw2gimi.h"
#include "file_reader.h"
#include "file_writer.h"
#include "image_factory.h"

using namespace gimi;

// CLI API
void Raw2Gimi::write_image_from_memory(MainArgs args) {
  uint32_t width = args.extract_width();
  uint32_t height = args.extract_height();
  heif_compression_format compression = args.extract_compression();
  heif_colorspace colorspace = args.extract_colorspace();
  heif_chroma chroma = args.extract_chroma();
  int bit_depth = args.extract_bit_depth();
  string pixel_algorithm = args.pixel_algorithm;
  string output_filename = args.extract_output_filename();

  // Create Image
  // heif_context *ctx = encode_image_from_memory(compression, width, height, chroma, colorspace, bit_depth);
  ImageFactory imageFactory(width, height, chroma, colorspace, bit_depth);
  gimi::Image image = imageFactory.create();

  // WRITE
  // he(heif_context_write_to_file(ctx, output_filename.c_str()));
  // cout << "Created: " << output_filename << endl;
}

void Raw2Gimi::raw_to_gimi(MainArgs args) {
  string input_filename = args.input_filename;
  string output_filename = args.extract_output_filename();

  Raw2Gimi::raw_to_gimi(input_filename, output_filename);
}

void Raw2Gimi::heif_to_gimi(MainArgs args) {
  cout << "TODO: heif_to_gimi()" << endl;
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
  he(heif_context_encode_image(ctx, img, encoder, nullptr, &handle));

  return ctx;
}

void Raw2Gimi::raw_to_gimi(const string &input_filename, const string &output_filename) {
  const gimi::Image image = FileReader::read_file(input_filename);
  gimi::FileWriter::write_image_to_file(image, output_filename);
}

// Helper Functions

void Raw2Gimi::he(struct heif_error error) {
  if (error.code) {
    printf("ERROR! - subcode: %d  Message: %s\n", error.subcode, error.message);
    exit(error.code);
  }
}
