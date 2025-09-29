#include "main_args.h"
#include "error_handler.h"
#include "options.h"

using namespace gimi;

const std::string MainArgs::default_output_filename = "out/output.heif";

// Constructor
MainArgs::MainArgs(int argc, const char *argv[]) {
  OptionHandler option_handler;
  bool help = false;

  // Action
  option_handler.add("a", "action", &action, "The action to perform. Default = heif_to_gimi");
  option_handler.add("d", "debug", &debug, "Used for debugging");
  option_handler.add("i", "input", &input_filename, "Path to input file");
  option_handler.add("o", "output", &output_filename, "Path to output file");

  // Output
  option_handler.add("e", "codec", &codec, "raw (default), avc, hevc, j2k, av1");
  option_handler.add("", "chroma", &chroma, "rgb (default), mono, 444, 422, 420, 411");
  option_handler.add("", "interleave", &interleave, "interleaved (default) or planar");
  option_handler.add("", "pixel_algorithm", &pixel_algorithm, "Specify function to create image. Default: solid. {stripes, random, four_squares}");
  option_handler.add("", "width", &width, "Specify width of image");
  option_handler.add("", "height", &height, "Specify height of image");
  option_handler.add("", "scale", &scale_factor, "Specify scale factor for scaling. e.g. .5 will scale the image in half.");

  // Gridding
  option_handler.add("", "rows", &rows, "Number of rows for the grid");
  option_handler.add("", "columns", &columns, "Number of columns for the grid");
  option_handler.add("", "tile-directory", &tile_directory, "Folder for HEIC tiles to form the grid.");
  option_handler.add("", "layers", &layers, "Specifies the number of desired layers in an image pyramid.");

  // Flags
  option_handler.addFlag("b", "long_b", &flag1, "Test Bool.");
  option_handler.addFlag("h", "help", &help, "Print usage and exit.");

  // Parse Options
  vector<string> urls = option_handler.parse(argc, argv); // Update Option flags & variables with user input

  if (help || urls.size() == 0) {
    option_handler.printHelp(stdout);
    exit(0);
  }

  input_filename = urls[0];
  if (urls.size() >= 2) {
    output_filename = urls[1];
  }
}

MainArgs::MainArgs() {
  input_filename = "";
  output_filename = default_output_filename;
  input_rdf_filename = "";
  codec = "hevc";
  chroma = "rgb";
  interleave = "interleaved";
  pixel_type = "8";
  action = "heif_to_gimi";
  pixel_algorithm = "solid";
  width = "64";
  height = "64";
  rows = "2";
  columns = "2";
  tile_directory = "in";
  scale_factor = "1.0";
  layers = "1";
}

// Extractors

string MainArgs::extract_output_filename() {
  if (output_filename.empty()) {
    return "out/output.heif"; // default output filename
  }
  return output_filename;
}

Codec MainArgs::extract_codec() {
  // TODO: make an enum for codec
  if (codec == "unc" || codec == "uncompressed" || codec == "raw" || codec.empty()) {
    return Codec::raw;
  } else if (codec == "avc" || codec == "h264") {
    return Codec::avc;
  } else if (codec == "hevc" || codec == "h265") {
    return Codec::hevc;
  } else if (codec == "j2k") {
    return Codec::j2k;
  } else if (codec == "av1") {
    return Codec::av1;
  } else {
    throw_error("Unsupported encoding format: %s", codec.c_str());
  }
  return Codec::raw;
}

gimi::Chroma MainArgs::extract_chroma() {
  if (chroma == "rgb" || chroma.empty()) {
    return Chroma::rgb;
  } else if (chroma_is_mono(chroma)) {
    return Chroma::gray;
  } else if (chroma == "444" || chroma == "yuv") {
    return Chroma::yuv_444;
  } else if (chroma == "422") {
    return Chroma::yuv_422;
  } else if (chroma == "420") {
    return Chroma::yuv_420;
  } else if (chroma == "411") {
    return Chroma::yuv_411;
  } else {
    throw_error("Unsupported chroma format: %s", chroma.c_str());
  }
  return Chroma::rgb;
}

gimi::Interleave MainArgs::extract_interleave() {
  if (interleave == "interleaved" || interleave.empty()) {
    return Interleave::interleaved;
  } else if (interleave == "planar") {
    return Interleave::planar;
  } else {
    throw_error("Unsupported interleaving format: %s", interleave.c_str());
  }
  return Interleave::interleaved;
}

PixelType MainArgs::extract_pixel_type() {
  if (pixel_type == "8" || pixel_type.empty()) {
    return PixelType::uint8;
  } else if (pixel_type == "10") {
    return PixelType::uint10;
  } else if (pixel_type == "12") {
    return PixelType::uint12;
  } else if (pixel_type == "14") {
    return PixelType::uint14;
  } else if (pixel_type == "16") {
    return PixelType::uint16;
  } else if (pixel_type == "int8") {
    return PixelType::int8;
  } else if (pixel_type == "int16") {
    return PixelType::int16;
  } else if (pixel_type == "float32") {
    return PixelType::float32;
  } else if (pixel_type == "complex") {
    return PixelType::complex;
  } else {
    throw_error("Unsupported pixel type: %s", pixel_type.c_str());
  }
  return PixelType::uint8;
}

uint32_t MainArgs::extract_width() {
  return MainArgs::string_to_int(width, default_dimensions);
}

uint32_t MainArgs::extract_height() {
  return MainArgs::string_to_int(height, default_dimensions);
}

uint32_t MainArgs::extract_rows() {
  return MainArgs::string_to_int(rows, 2);
}

uint32_t MainArgs::extract_columns() {
  return MainArgs::string_to_int(columns, 2);
}

uint32_t MainArgs::extract_layers() {
  return MainArgs::string_to_int(layers, 1);
}

double MainArgs::extract_scale_factor() {
  try {
    return stod(scale_factor);
  } catch (const invalid_argument &ia) {
    throw_error("Invalid scale value: %s", ia.what());
  } catch (const out_of_range &oor) {
    throw_error("Scale value is out of range: %s", oor.what());
  }
  return 0;
}

MainArgsAction MainArgs::extract_action() {
  if (action == "create_image") {
    return MainArgsAction::CREATE_IMAGE;
  } else if (action == "create_grid") {
    return MainArgsAction::CREATE_GRID;
  } else if (action == "create_sequence") {
    return MainArgsAction::CREATE_SEQUENCE;
  } else if (action == "image_to_gimi") {
    return MainArgsAction::IMAGE_TO_GIMI;
  } else if (action == "raw_to_gimi") {
    return MainArgsAction::RAW_TO_GIMI;
  } else if (action == "image_to_tiles") {
    return MainArgsAction::IMAGE_TO_TILES;
  } else if (action == "heif_to_gimi") {
    return MainArgsAction::HEIF_TO_GIMI;
  } else if (action == "write_image_with_rdf") {
    return MainArgsAction::WRITE_IMAGE_WITH_RDF;
  } else if (action == "tiles_to_grid") {
    return MainArgsAction::TILES_TO_GRID;
  } else if (action == "generate_sample_files") {
    return MainArgsAction::GENERATE_SAMPLE_FILES;
  } else {
    return MainArgsAction::HEIF_TO_GIMI; // default action
  }
}

void MainArgs::print() {
  cout << "input_filename = " << input_filename << endl;
  cout << "output_filename = " << output_filename << endl;
  cout << "encoding = " << codec << endl;
  cout << "action = " << action << endl;
  cout << "debug = " << debug << endl;
  cout << "flag1 = " << flag1 << endl;
};

// Setters

MainArgs &MainArgs::set_input_filename(const string &input_filename) {
  this->input_filename = input_filename;
  return *this;
}

MainArgs &MainArgs::set_input_rdf_filename(const string &input_rdf_filename) {
  this->input_rdf_filename = input_rdf_filename;
  return *this;
}

MainArgs &MainArgs::set_output_filename(const string &output_filename) {
  this->output_filename = output_filename;
  return *this;
}

MainArgs &MainArgs::set_output_filename() {

  output_filename = "out/";
  output_filename.append(codec);
  output_filename.append("_" + chroma);

  if (!chroma_is_mono(chroma))
    output_filename.append("_" + interleave); // interleave is not applicable when there is only 1 band

  output_filename.append("_" + pixel_type + "bit");
  output_filename.append("_" + width + "x" + height);

  if (action == "create_grid") {
    output_filename.append("_" + rows + "x" + columns + "_grid");
  }

  string extension = ".heif";
  if (action == "create_sequence") {
    extension = ".mp4";
  } else if (codec == "av1") {
    extension = ".avif";
  }

  output_filename.append(extension);
  return *this;
}

MainArgs &MainArgs::set_codec(const string &codec) {
  this->codec = codec;
  return *this;
}

MainArgs &MainArgs::set_chroma(const string &chroma) {
  this->chroma = chroma;
  return *this;
}

MainArgs &MainArgs::set_interleave(const string &interleave) {
  this->interleave = interleave;
  return *this;
}

MainArgs &MainArgs::set_pixel_type(const string &pixel_type) {
  this->pixel_type = pixel_type;
  return *this;
}

MainArgs &MainArgs::set_action(const string &action) {
  this->action = action;
  return *this;
}

MainArgs &MainArgs::set_debug(const string &debug) {
  this->debug = debug;
  return *this;
}

MainArgs &MainArgs::set_pixel_algorithm(const string &pixel_algorithm) {
  this->pixel_algorithm = pixel_algorithm;
  return *this;
}

MainArgs &MainArgs::set_width(const string &width) {
  this->width = width;
  return *this;
}

MainArgs &MainArgs::set_height(const string &height) {
  this->height = height;
  return *this;
}

MainArgs &MainArgs::set_rows(const string &rows) {
  this->rows = rows;
  return *this;
}

MainArgs &MainArgs::set_columns(const string &columns) {
  this->columns = columns;
  return *this;
}

MainArgs &MainArgs::set_tile_directory(const string &tile_directory) {
  this->tile_directory = tile_directory;
  return *this;
}

MainArgs &MainArgs::set_scale_factor(const string &scale_factor) {
  this->scale_factor = scale_factor;
  return *this;
}

// Helper Functions

uint32_t MainArgs::string_to_int(string str, uint32_t default_value) {
  if (str == "") {
    return default_value;
  }

  try {
    int value = stoi(str);
    return value;
  } catch (const invalid_argument &e) {
    cout << "Invalid argument: " << e.what() << endl;
    exit(1);
  } catch (const out_of_range &e) {
    cout << "Out of range: " << e.what() << endl;
    exit(1);
  } catch (...) {
    cout << "Unknown error" << endl;
    exit(1);
  }
}

bool MainArgs::chroma_is_mono(const string &chroma) {
  if (chroma == "gray" ||
      chroma == "grey" ||
      chroma == "mono" ||
      chroma == "monochrome") {
    return true;
  }
  return false;
}