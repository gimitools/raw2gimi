#include "raw2gimi.h"
#include "file_reader.h"
#include "gimifier.h"
#include "image_factory.h"
#include "model/pixel_formats.h"

using namespace gimi;

// CLI API
void Raw2Gimi::write_image_from_memory(MainArgs args) {
  uint32_t width = args.extract_width();
  uint32_t height = args.extract_height();
  int bit_depth = args.extract_bit_depth();
  string pixel_algorithm = args.pixel_algorithm;
  string output_filename = args.extract_output_filename();
  Chroma chroma = args.extract_chroma();
  Interleave interleave = args.extract_interleave();
  Codec codec = args.extract_codec();

  // Create RawImage
  ImageFactory imageFactory(width, height, chroma, interleave, bit_depth);
  gimi::RawImage image = imageFactory.create_image();

  // WRITE
  Gimifier::write_to_file(image, codec, output_filename);
  cout << "Created: " << output_filename << endl;
}

void Raw2Gimi::raw_to_gimi(MainArgs args) {
  const string input_filename = args.input_filename;
  const string output_filename = args.extract_output_filename();

  Raw2Gimi::raw_to_gimi(input_filename, output_filename);
}

void Raw2Gimi::heif_to_gimi(MainArgs args) {
  cout << "TODO: heif_to_gimi()" << endl;
  exit(1);
}

// Primary Functions
void Raw2Gimi::raw_to_gimi(const string &input_filename, const string &output_filename) {
  cout << "TODO: raw_to_gimi()" << endl;
  exit(1);
}