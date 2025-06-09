#include "raw2gimi.h"
#include "error_handler.h"
#include "file_reader.h"
#include "gimifier.h"
#include "image_factory.h"
#include "model/pixel_formats.h"

using namespace gimi;

// CLI API
void Raw2Gimi::write_image_from_memory(MainArgs args) {
  uint32_t width = args.extract_width();
  uint32_t height = args.extract_height();
  PixelType pixel_type = args.extract_pixel_type();
  Chroma chroma = args.extract_chroma();
  Interleave interleave = args.extract_interleave();
  Codec codec = args.extract_codec();
  string output_filename = args.extract_output_filename();

  // Create RawImage
  ImageFactory imageFactory(width, height, chroma, interleave, pixel_type);
  gimi::RawImage image = imageFactory.create_image();

  WriteOptions options = Raw2Gimi::create_write_options(args);

  // Write to File
  Gimifier::write_to_file(image, options);
  cout << "Created: " << output_filename << endl;
}

void Raw2Gimi::write_grid_from_memory(MainArgs args) {
  uint32_t width = args.extract_width();
  uint32_t height = args.extract_height();
  PixelType pixel_type = args.extract_pixel_type();
  Chroma chroma = args.extract_chroma();
  Interleave interleave = args.extract_interleave();
  Codec codec = args.extract_codec();
  string output_filename = args.extract_output_filename();

  // Create tiles
  ImageFactory imageFactory(width, height, chroma, interleave, pixel_type);
  vector<RawImage> tiles = imageFactory.create_tiles(2, 2);

  WriteOptions options = Raw2Gimi::create_write_options(args);

  // Write to File
  Gimifier::write_grid_to_file(tiles, options);
  cout << "Created: " << output_filename << endl;
}

void Raw2Gimi::raw_to_gimi(MainArgs args) {
  const string input_filename = args.input_filename;
  const string output_filename = args.extract_output_filename();

  Raw2Gimi::raw_to_gimi(input_filename, output_filename);
}

void Raw2Gimi::heif_to_gimi(MainArgs args) {
  throw_error("Function not yet implemented");
}

// Primary Functions
void Raw2Gimi::raw_to_gimi(const string &input_filename, const string &output_filename) {
  throw_error("Function not yet implemented");
}

// Helper Functions

WriteOptions Raw2Gimi::create_write_options(MainArgs args) {
  WriteOptions options;
  options.output_filename = args.extract_output_filename();
  options.codec = args.extract_codec();
  return options;
}