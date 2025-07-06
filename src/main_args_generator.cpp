#include "main_args_generator.h"
#include "raw2gimi.h"

using namespace gimi;

// Public

const vector<MainArgs> MainArgsGenerator::generate_main_args() {
  vector<MainArgs> all_args;

  MainArgsGenerator::add_args_simple(all_args);
  MainArgsGenerator::add_args_grid(all_args);
  MainArgsGenerator::add_args_sequence(all_args);

  return all_args;
}

// Protected

MainArgs MainArgsGenerator::create_args(string encoding, string chroma, string interleave, string pixel_type, string width, string height) {
  MainArgs args;
  args.set_action("create_image")
      .set_codec(encoding)
      .set_chroma(chroma)
      .set_interleave(interleave)
      .set_pixel_type(pixel_type)
      .set_width(width)
      .set_height(height)
      .set_output_filename();
  return args;
}

void MainArgsGenerator::add_args_simple(vector<MainArgs> &v) {
  string codec = "hevc";
  string chroma = "rgb";
  string interleave = "interleaved";
  string pixel_type = "8";
  string width = "64";
  string height = "64";
  MainArgs args;

  args.set_action("create_image");

  // Mono
  args.set_chroma("gray")
      .set_output_filename();
  v.push_back(args);

  // Default
  // v.push_back(create_args(codec, chroma, interleave, pixel_type, height, width));

  // Testing
  // v.push_back(create_args("unc", "rgb", interleave, "10", height, width));

  // Codec variations
  v.push_back(create_args("unc", chroma, interleave, pixel_type, height, width));
  v.push_back(create_args("hevc", chroma, interleave, pixel_type, height, width));
  v.push_back(create_args("av1", chroma, interleave, pixel_type, height, width));
  // Warning! OpenJPEG not compiled in!
  // v.push_back(create_args("j2k", chroma, interleave, pixel_type, height, width));

  // HDR
  v.push_back(create_args("unc", chroma, interleave, "10", height, width));
  v.push_back(create_args("unc", chroma, interleave, "12", height, width));
  v.push_back(create_args("unc", chroma, interleave, "14", height, width));
  v.push_back(create_args("unc", chroma, interleave, "16", height, width));

  // Formats variations
  // v.push_back(create_args(codec, "rgb", "interleaved", pixel_type, height, width));
  // v.push_back(create_args(codec, "rgb", "planar", pixel_type, height, width));
  v.push_back(create_args(codec, "444", "planar", pixel_type, height, width));
  // v.push_back(create_args(codec, "422", "planar", pixel_type, height, width));
  // v.push_back(create_args(codec, "420", "planar", pixel_type, height, width));
};

void MainArgsGenerator::add_args_grid(vector<MainArgs> &v) {
  MainArgs args;
  args.set_action("create_grid");

  // 2x2 Grid
  args.set_rows("2")
      .set_columns("2")
      .set_codec("hevc")
      .set_output_filename();
  v.push_back(args);

  // 10x10 Grid
  args.set_rows("10")
      .set_columns("10")
      .set_codec("hevc")
      .set_output_filename();
  v.push_back(args);

  // MainArgs uncC_internal;
  // MainArgs tili_item;
}

void MainArgsGenerator::add_args_sequence(vector<MainArgs> &v) {
  MainArgs args;

  // TODO: automate output_filename creation

  // Defaults
  args.set_action("create_sequence")
      .set_chroma("rgb")
      .set_interleave("planar")
      .set_pixel_type("8");

  args.set_codec("hevc")
      .set_output_filename();
  v.push_back(args);

  args.set_codec("av1")
      .set_output_filename();
  v.push_back(args);
}
