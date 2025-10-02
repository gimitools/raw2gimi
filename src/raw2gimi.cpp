#include "raw2gimi.h"
#include "error_handler.h"
#include "file_reader.h"
#include "gimifier.h"
#include "image_factory.h"
#include "image_processor.h"
#include "main_args_generator.h"
#include "model/iso_file.h"
#include "model/pixel_formats.h"

using namespace gimi;

// Constructor

Raw2Gimi::Raw2Gimi(MainArgs args) {
  // Decouple User input from the Raw2Gimi Class.
  m_action = args.extract_action();
  m_width = args.extract_width();
  m_height = args.extract_height();
  m_rows = args.extract_rows();
  m_columns = args.extract_columns();
  m_pixel_type = args.extract_pixel_type();
  m_chroma = args.extract_chroma();
  m_interleave = args.extract_interleave();
  m_codec = args.extract_codec();
  m_output_filename = args.extract_output_filename();
  m_image_name = args.extract_image_name();
  m_sidecar_filename = args.sidecar_filename;
  m_sidecar_type = args.extract_sidecar_type();
  m_input_filename = args.input_filename;
}

// Primary API Function

void Raw2Gimi::execute_action() {

  WriteOptions options = create_write_options();

  switch (m_action) {
  case MainArgsAction::CREATE_GRID:
    return create_grid(options);
  case MainArgsAction::CREATE_SEQUENCE:
    return create_sequence(options);
  case MainArgsAction::IMAGE_TO_GIMI:
    return image_to_gimi(options);
  case MainArgsAction::RAW_TO_GIMI:
    return raw_to_gimi(options);
  case MainArgsAction::HEIF_TO_GIMI:
    return heif_to_gimi(options);
  case MainArgsAction::IMAGE_TO_TILES:
    return image_to_tiles(options);
  case MainArgsAction::WRITE_IMAGE_WITH_RDF:
    return write_image_with_rdf(options);
  case MainArgsAction::GENERATE_SAMPLE_FILES:
    return generate_sample_files(options);
  default:
    throw_error("Unrecognized action: %d", m_action);
  }
}

// CLI API

void Raw2Gimi::create_image(WriteOptions options) {

  // Create RawImage
  ImageFactory imageFactory(m_width, m_height, m_chroma, m_interleave, m_pixel_type);
  gimi::RawImage image = imageFactory.create_image();

  // Write to File
  Gimifier::write_to_file(image, options);
  cout << "Created: " << m_output_filename << endl;
}

void Raw2Gimi::create_grid(WriteOptions options) {
  // Create tiles
  ImageFactory imageFactory(m_width, m_height, m_chroma, m_interleave, m_pixel_type);
  RawImageGrid grid = imageFactory.create_tiles(m_columns, m_rows);

  // Write to File
  Gimifier::write_grid_to_file(grid, options);
  cout << "Created: " << m_output_filename << endl;
}

void Raw2Gimi::create_sequence(WriteOptions options) {
  // Create sequence
  ImageFactory imageFactory(m_width, m_height, m_chroma, m_interleave, m_pixel_type);
  imageFactory.set_frame_count(60);
  vector<RawImage> sequence = imageFactory.create_sequence("solid");

  Gimifier::write_video_to_file(sequence, options);
  cout << "Created: " << m_output_filename << endl;
}

void Raw2Gimi::image_to_gimi(WriteOptions options) {
  auto rawImage = FileReader::read_file(m_input_filename);

  IsoFile isoFile;
  isoFile.add_image(rawImage);

  WriteOptions writeOptions = create_write_options();

  Gimifier::write_to_file(isoFile, writeOptions);
  cout << "Created: " << m_output_filename << endl;
}

void Raw2Gimi::raw_to_gimi(WriteOptions options) {
  Raw2Gimi::raw_to_gimi(m_input_filename, m_output_filename);
}

void Raw2Gimi::heif_to_gimi(WriteOptions options) {
  Raw2Gimi::heif_to_gimi(m_input_filename, options);
}

void Raw2Gimi::image_to_tiles(WriteOptions options) {
  auto rawImage = FileReader::read_file(m_input_filename);
  uint32_t rows = options.rows;
  uint32_t columns = options.columns;
  RawImageGrid grid = ImageProcessor::image_to_grid(rawImage, rows, columns);

  if (!m_sidecar_filename.empty()) {
    CsvFile csv = FileReader::read_csv(m_sidecar_filename);
    WriteOptions rdf_options = create_write_options();
    rdf_options.output_filename = "out/rdf_output.ttl";
    Gimifier::write_unreal_to_rdf(grid, csv, rdf_options);
  }

  // Gimifier::write_grid_to_file(grid, options);
  // cout << "Created: " << options.output_filename << endl;
  cout << "Warning!: commented out writing the grid to file for testing purposes."
       << endl;
}

void Raw2Gimi::write_image_with_rdf(WriteOptions options) {
  Gimifier::debug();
}

void Raw2Gimi::generate_sample_files(WriteOptions options) {

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

void Raw2Gimi::heif_to_gimi(const string &input_filename, WriteOptions write_options) {

  string output_filename = write_options.output_filename;

  // Read HEIF File
  auto rawImage = FileReader::read_heif(input_filename);

  // Write to File
  Gimifier::write_to_file(rawImage, write_options);
  cout << "Created: " << output_filename << endl;
}

// Helper Functions

WriteOptions Raw2Gimi::create_write_options() {
  WriteOptions options;
  options.output_filename = m_output_filename;
  options.codec = m_codec;
  options.rows = m_rows;
  options.columns = m_columns;
  options.image_name = m_image_name;
  return options;
}