#include "main_args.h"
#include "options.h"

// Constructor
MainArgs::MainArgs(int argc, const char *argv[]) {
  OptionHandler option_handler;
  bool help = false;

  // Action
  option_handler.add("a", "action", &action, "The action to perform. Default = heif_to_gimi");
  option_handler.add("d", "debug", &debug, "Used for debugging");
  option_handler.add("i", "input", &input_filename, "Path to input file");

  // Output
  option_handler.add("e", "encoding", &encoding, "Specify how the to encode the output file. Default = hevc");
  option_handler.add("", "colorspace", &colorspace, "rgb or mono");
  option_handler.add("", "chroma", &chroma, "Specify chroma format. Default: rgb. {mono, 444, 422, 420, rgb, rgba, rrggbb_be, rrggbbaa_be, rrggbb_le, rrggbbaa_le}");
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
  // if (urls.size() >= 3) {
  //   args.action = urls[2];
  // }
}

string MainArgs::extract_output_filename() {
  if (output_filename.empty()) {
    return "out/output.heif"; // default output filename
  }
  return output_filename;
}

heif_compression_format MainArgs::extract_compression() {
  if (encoding == "unc" || encoding == "uncompressed") {
    return heif_compression_uncompressed;
  } else if (encoding == "avc") {
    return heif_compression_AVC;
  } else if (encoding == "j2k") {
    return heif_compression_JPEG2000;
  } else if (encoding == "av1") {
    return heif_compression_AV1;
  } else {
    return heif_compression_HEVC;
  }
}

heif_colorspace MainArgs::extract_colorspace() {
  if (colorspace == "rgb" || colorspace == "RGB") {
    return heif_colorspace_RGB;
  } else if (colorspace == "mono" || colorspace == "monochrome") {
    return heif_colorspace_monochrome;
  } else if (colorspace == "yuv") {
    return heif_colorspace_YCbCr;
  } else {
    return heif_colorspace_RGB;
  }
}

heif_chroma MainArgs::extract_chroma() {
  if (chroma == "gray" || chroma == "mono") {
    return heif_chroma_monochrome;
  } else if (chroma == "444" || chroma == "planar") {
    return heif_chroma_444;
  } else if (chroma == "422") {
    return heif_chroma_422;
  } else if (chroma == "420") {
    return heif_chroma_420;
  } else if (chroma == "rgb") {
    if (bit_depth == "8" || bit_depth == "") {
      return heif_chroma_interleaved_RGB;
    } else {
      return heif_chroma_interleaved_RRGGBB_BE;
    }
    return heif_chroma_interleaved_RGB;
  } else if (chroma == "rgba") {
    if (bit_depth == "8" || bit_depth == "") {
      return heif_chroma_interleaved_RGBA;
    } else {
      return heif_chroma_interleaved_RRGGBBAA_BE;
    }
  } else if (chroma == "rrggbb_be") {
    return heif_chroma_interleaved_RRGGBB_BE;
  } else if (chroma == "rrggbbaa_be") {
    return heif_chroma_interleaved_RRGGBBAA_BE;
  } else if (chroma == "rrggbb_le") {
    return heif_chroma_interleaved_RRGGBB_LE;
  } else if (chroma == "rrggbbaa_le") {
    return heif_chroma_interleaved_RRGGBBAA_LE;
  } else {
    return heif_chroma_interleaved_RGB;
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
    return std::stod(scale_factor);
  } catch (const std::invalid_argument &ia) {
    std::cerr << "Invalid scale value: " << ia.what() << std::endl;
    exit(1);
  } catch (const std::out_of_range &oor) {
    std::cerr << "Scale value is out of range: " << oor.what() << std::endl;
    exit(1);
  }
}

void MainArgs::print() {
  cout << "input_filename = " << input_filename << endl;
  cout << "output_filename = " << output_filename << endl;
  cout << "encoding = " << encoding << endl;
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
  } catch (const std::invalid_argument &e) {
    std::cout << "Invalid argument: " << e.what() << std::endl;
    exit(1);
  } catch (const std::out_of_range &e) {
    std::cout << "Out of range: " << e.what() << std::endl;
    exit(1);
  } catch (...) {
    std::cout << "Unknown error" << std::endl;
    exit(1);
  }
}
