#include "raw2gimi.h"
#include "error_handler.h"
#include "file_reader.h"
#include "gimifier.h"
#include "image_factory.h"
#include "model/pixel_formats.h"

using namespace gimi;

// Constructor

Raw2Gimi::Raw2Gimi(MainArgs args) {
  m_width = args.extract_width();
  m_height = args.extract_height();
  m_rows = args.extract_rows();
  m_columns = args.extract_columns();
  m_pixel_type = args.extract_pixel_type();
  m_chroma = args.extract_chroma();
  m_interleave = args.extract_interleave();
  m_codec = args.extract_codec();
  m_output_filename = args.extract_output_filename();
  m_input_filename = args.input_filename;
}

// CLI API
void Raw2Gimi::write_image() {

  // Create RawImage
  ImageFactory imageFactory(m_width, m_height, m_chroma, m_interleave, m_pixel_type);
  gimi::RawImage image = imageFactory.create_image();

  WriteOptions options = create_write_options();

  // Write to File
  Gimifier::write_to_file(image, options);
  cout << "Created: " << m_output_filename << endl;
}

void Raw2Gimi::write_grid() {
  // Create tiles
  ImageFactory imageFactory(m_width, m_height, m_chroma, m_interleave, m_pixel_type);
  RawImageGrid grid = imageFactory.create_tiles(m_columns, m_rows);

  WriteOptions options = create_write_options();

  // Write to File
  Gimifier::write_grid_to_file(grid, options);
  cout << "Created: " << m_output_filename << endl;
}

void Raw2Gimi::raw_to_gimi() {
  Raw2Gimi::raw_to_gimi(m_input_filename, m_output_filename);
}

void Raw2Gimi::heif_to_gimi() {
  throw_error("Function not yet implemented");
}

void Raw2Gimi::write_image_and_rdf() {
  throw_error("Function not yet implemented");
}

// Primary Functions

void Raw2Gimi::raw_to_gimi(const string &input_filename, const string &output_filename) {
  throw_error("Function not yet implemented");
}

// Helper Functions

WriteOptions Raw2Gimi::create_write_options() {
  WriteOptions options;
  options.output_filename = m_output_filename;
  options.codec = m_codec;
  return options;
}