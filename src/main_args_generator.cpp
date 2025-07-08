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

  add_args_simple_uncompressed(v);
  add_args_simple_hevc(v);
  add_args_simple_av1(v);
  add_args_simple_j2k(v);
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

  args.set_codec("unc")
      .set_chroma("mono")
      .set_output_filename();
  v.push_back(args);

  args.set_codec("hevc")
      .set_chroma("mono")
      .set_output_filename();
  v.push_back(args);

  args.set_codec("hevc")
      .set_chroma("rgb")
      .set_interleave("interleaved")
      .set_pixel_type("10") // See hvvC: bit_depth_luma_minus8 & bit_depth_chroma_minus8
      .set_output_filename();
  v.push_back(args);

  args.set_pixel_type("12")
      .set_output_filename();
  v.push_back(args);
}

void MainArgsGenerator::add_args_simple_uncompressed(vector<MainArgs> &v) {
  string width = "64";
  string height = "64";

  // RGB Interleaved
  v.push_back(create_args("unc", "rgb", "interleaved", "8", height, width));
  v.push_back(create_args("unc", "rgb", "interleaved", "10", height, width));
  v.push_back(create_args("unc", "rgb", "interleaved", "12", height, width));
  v.push_back(create_args("unc", "rgb", "interleaved", "14", height, width));
  v.push_back(create_args("unc", "rgb", "interleaved", "16", height, width));

  // RGB Planar
  v.push_back(create_args("unc", "rgb", "planar", "8", height, width));
  v.push_back(create_args("unc", "rgb", "planar", "10", height, width));
  v.push_back(create_args("unc", "rgb", "planar", "12", height, width));
  v.push_back(create_args("unc", "rgb", "planar", "14", height, width));
  v.push_back(create_args("unc", "rgb", "planar", "16", height, width));

  // Monochrome
  v.push_back(create_args("unc", "mono", "", "8", height, width));
  v.push_back(create_args("unc", "mono", "", "10", height, width));
  v.push_back(create_args("unc", "mono", "", "12", height, width));
  v.push_back(create_args("unc", "mono", "", "14", height, width));
  v.push_back(create_args("unc", "mono", "", "16", height, width));

  // YUV
}

void MainArgsGenerator::add_args_simple_hevc(vector<MainArgs> &v) {
  string width = "64";
  string height = "64";

  // RGB Interleaved
  v.push_back(create_args("hevc", "rgb", "interleaved", "8", height, width));
  v.push_back(create_args("hevc", "rgb", "interleaved", "10", height, width));
  v.push_back(create_args("hevc", "rgb", "interleaved", "12", height, width));
  // v.push_back(create_args("hevc", "rgb", "interleaved", "14", height, width));
  // v.push_back(create_args("hevc", "rgb", "interleaved", "16", height, width));

  // RGB Planar
  v.push_back(create_args("hevc", "rgb", "planar", "8", height, width));
  // v.push_back(create_args("hevc", "rgb", "planar", "10", height, width));
  // v.push_back(create_args("hevc", "rgb", "planar", "12", height, width));
  // v.push_back(create_args("hevc", "rgb", "planar", "14", height, width));
  // v.push_back(create_args("hevc", "rgb", "planar", "16", height, width));

  // Monochrome
  v.push_back(create_args("hevc", "mono", "", "8", height, width));
  // v.push_back(create_args("hevc", "mono", "", "10", height, width));
  // v.push_back(create_args("hevc", "mono", "", "12", height, width));
  // v.push_back(create_args("hevc", "mono", "", "14", height, width));
  // v.push_back(create_args("hevc", "mono", "", "16", height, width));

  // YUV
  // v.push_back(create_args("hevc", "444", "interleaved", "8", height, width));
  // v.push_back(create_args("hevc", "422", "interleaved", "8", height, width));
  // v.push_back(create_args("hevc", "420", "interleaved", "8", height, width));
  v.push_back(create_args("hevc", "444", "planar", "8", height, width));
  v.push_back(create_args("hevc", "422", "planar", "8", height, width));
  v.push_back(create_args("hevc", "420", "planar", "8", height, width));
}
void MainArgsGenerator::add_args_simple_av1(vector<MainArgs> &v) {
  string width = "64";
  string height = "64";

  // RGB Interleaved
  v.push_back(create_args("av1", "rgb", "interleaved", "8", height, width));
  v.push_back(create_args("av1", "rgb", "interleaved", "10", height, width));
  v.push_back(create_args("av1", "rgb", "interleaved", "12", height, width));
  // v.push_back(create_args("av1", "rgb", "interleaved", "14", height, width));
  // v.push_back(create_args("av1", "rgb", "interleaved", "16", height, width));

  // RGB Planar
  v.push_back(create_args("av1", "rgb", "planar", "8", height, width));
  // v.push_back(create_args("av1", "rgb", "planar", "10", height, width));
  // v.push_back(create_args("av1", "rgb", "planar", "12", height, width));
  // v.push_back(create_args("av1", "rgb", "planar", "14", height, width));
  // v.push_back(create_args("av1", "rgb", "planar", "16", height, width));

  // Monochrome
  v.push_back(create_args("av1", "mono", "", "8", height, width));
  // v.push_back(create_args("av1", "mono", "", "10", height, width));
  // v.push_back(create_args("av1", "mono", "", "12", height, width));
  // v.push_back(create_args("av1", "mono", "", "14", height, width));
  // v.push_back(create_args("av1", "mono", "", "16", height, width));

  // YUV
  // v.push_back(create_args("av1", "444", "interleaved", "8", height, width));
  // v.push_back(create_args("av1", "422", "interleaved", "8", height, width));
  // v.push_back(create_args("av1", "420", "interleaved", "8", height, width));
  v.push_back(create_args("av1", "444", "planar", "8", height, width));
  v.push_back(create_args("av1", "422", "planar", "8", height, width));
  v.push_back(create_args("av1", "420", "planar", "8", height, width));
}

void MainArgsGenerator::add_args_simple_j2k(vector<MainArgs> &v) {
  // Warning! OpenJPEG not compiled in!
  // v.push_back(create_args("j2k", chroma, interleave, pixel_type, height, width));
}