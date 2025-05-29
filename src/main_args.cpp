#include "main_args.h"
#include "options.h"

using namespace gimi;

// Constructor
MainArgs::MainArgs(int argc, const char *argv[]) {
  OptionHandler option_handler;
  bool help = false;

  // Action
  option_handler.add("a", "action", &action, "The action to perform. Default = heif_to_gimi");
  option_handler.add("d", "debug", &debug, "Used for debugging");
  option_handler.add("i", "input", &input_filename, "Path to input file");

  // Output
  option_handler.add("e", "codec", &codec, "raw (default), avc, hevc, j2k, av1");
  option_handler.add("", "sampling", &sampling, "444 (default), 422, 420, 411");
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

  // input_filename = urls[0];
  // if (urls.size() >= 2) {
  //   output_filename = urls[1];
  // }
}

string MainArgs::extract_output_filename() {
  if (output_filename.empty()) {
    return "out/output.heif"; // default output filename
  }
  return output_filename;
}

string MainArgs::extract_codec() {
  // TODO: make an enum for codec
  if (codec == "unc" || codec == "uncompressed" || codec == "raw" || codec.empty()) {
    return "raw";
  } else if (codec == "avc" || codec == "h264") {
  } else if (codec == "hevc" || codec == "h265") {
    return codec;
  } else if (codec == "j2k") {
    return codec;
  } else if (codec == "av1") {
    return codec;
  } else {
    cerr << "Unsupported encoding format: " << codec << endl;
    exit(1);
  }
}

gimi::Sampling MainArgs::extract_sampling() {
  if (sampling == "444" || sampling.empty()) {
    return Sampling::yuv_444;
  } else if (sampling == "422") {
    return Sampling::yuv_422;
  } else if (sampling == "420") {
    return Sampling::yuv_420;
  } else if (sampling == "411") {
    return Sampling::yuv_411;
  } else {
    cerr << "Unsupported sampling format: " << sampling << endl;
    exit(1);
  }
}

gimi::Interleave MainArgs::extract_interleave() {
  if (interleave == "interleaved" || interleave.empty()) {
    return Interleave::interleaved;
  } else if (interleave == "planar") {
    return Interleave::planar;
  } else if (interleave == "mixed") {
    return Interleave::mixed;
  } else if (interleave == "row") {
    return Interleave::row;
  } else if (interleave == "tile_component") {
    return Interleave::tile_component;
  } else if (interleave == "multi_y_pixel") {
    return Interleave::multi_y_pixel;
  } else {
    cerr << "Unsupported interleaving format: " << interleave << endl;
    exit(1);
  }
}

uint32_t MainArgs::extract_bit_depth() {
  return MainArgs::string_to_int(bit_depth, default_bit_depth);
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
    cerr << "Invalid scale value: " << ia.what() << endl;
    exit(1);
  } catch (const out_of_range &oor) {
    cerr << "Scale value is out of range: " << oor.what() << endl;
    exit(1);
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
