#include "raw2gimi.h"
#include "error_handler.h"
#include "file_reader.h"
#include "gimifier.h"
#include "image_factory.h"
#include "main_args_generator.h"
#include "model/pixel_formats.h"

using namespace gimi;

// Constructor

Raw2Gimi::Raw2Gimi(MainArgs args) {
  m_action = args.action;
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

// Primary API Function

void Raw2Gimi::execute_action() {
  if (m_action == "create_image" || m_action.empty()) {
    create_image();
  } else if (m_action == "create_grid") {
    create_grid();
  } else if (m_action == "create_sequence") {
    create_sequence();
  } else if (m_action == "raw_to_gimi") {
    raw_to_gimi();
  } else if (m_action == "heif_to_gimi") {
    heif_to_gimi();
  } else if (m_action == "write_image_with_rdf") {
    write_image_with_rdf();
  } else if (m_action == "generate_sample_files") {
    generate_sample_files();
  } else {
    throw_error("Unrecognized action: %s", m_action);
  }
}

// CLI API

void Raw2Gimi::create_image() {

  // Create RawImage
  ImageFactory imageFactory(m_width, m_height, m_chroma, m_interleave, m_pixel_type);
  gimi::RawImage image = imageFactory.create_image();

  WriteOptions options = create_write_options();

  // Write to File
  Gimifier::write_to_file(image, options);
  cout << "Created: " << m_output_filename << endl;
}

void Raw2Gimi::create_grid() {
  // Create tiles
  ImageFactory imageFactory(m_width, m_height, m_chroma, m_interleave, m_pixel_type);
  RawImageGrid grid = imageFactory.create_tiles(m_columns, m_rows);

  WriteOptions options = create_write_options();

  // Write to File
  Gimifier::write_grid_to_file(grid, options);
  cout << "Created: " << m_output_filename << endl;
}

void Raw2Gimi::create_sequence() {
  // Create sequence
  ImageFactory imageFactory(m_width, m_height, m_chroma, m_interleave, m_pixel_type);
  imageFactory.set_frame_count(60);
  vector<RawImage> sequence = imageFactory.create_sequence("solid");

  WriteOptions options = create_write_options();

  Gimifier::write_video_to_file(sequence, options);
  cout << "Created: " << m_output_filename << endl;
}

void Raw2Gimi::raw_to_gimi() {
  Raw2Gimi::raw_to_gimi(m_input_filename, m_output_filename);
}

void Raw2Gimi::heif_to_gimi() {
  throw_error("Function not yet implemented");
}

void Raw2Gimi::write_image_with_rdf() {
  Gimifier::debug();
}

void Raw2Gimi::generate_sample_files() {

  vector<MainArgs> all_args = MainArgsGenerator::generate_main_args();
  for (const MainArgs &args : all_args) {
    Raw2Gimi raw2gimi(args);
    raw2gimi.execute_action();
  }
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