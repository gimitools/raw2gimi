#include "main_args_generator.h"
#include "raw2gimi.h"

using namespace gimi;

// Public

const vector<MainArgs> MainArgsGenerator::generate_main_args() {
  vector<MainArgs> all_args;

  bool debug = true;
  if (debug) {
    MainArgsGenerator::add_args_sequence(all_args);
  } else {
    MainArgsGenerator::add_args_simple(all_args);
    MainArgsGenerator::add_args_grid(all_args);
    MainArgsGenerator::add_args_sequence(all_args);
  }

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
  add_args_sequence_uncompressed(v);
  add_args_sequence_hevc(v);
  add_args_sequence_av1(v);
  add_args_sequence_j2k(v);
}

// Simple Images

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
  string width = "64";
  string height = "64";

  // RGB Interleaved
  v.push_back(create_args("j2k", "rgb", "interleaved", "8", height, width));
  v.push_back(create_args("j2k", "rgb", "interleaved", "10", height, width));
  v.push_back(create_args("j2k", "rgb", "interleaved", "12", height, width));
  v.push_back(create_args("j2k", "rgb", "interleaved", "14", height, width));
  v.push_back(create_args("j2k", "rgb", "interleaved", "16", height, width));

  // RGB Planar
  v.push_back(create_args("j2k", "rgb", "planar", "8", height, width));
  v.push_back(create_args("j2k", "rgb", "planar", "10", height, width));
  v.push_back(create_args("j2k", "rgb", "planar", "12", height, width));
  v.push_back(create_args("j2k", "rgb", "planar", "14", height, width));
  v.push_back(create_args("j2k", "rgb", "planar", "16", height, width));

  // Monochrome
  v.push_back(create_args("j2k", "mono", "", "8", height, width));
  v.push_back(create_args("j2k", "mono", "", "10", height, width));
  v.push_back(create_args("j2k", "mono", "", "12", height, width));
  v.push_back(create_args("j2k", "mono", "", "14", height, width));
  v.push_back(create_args("j2k", "mono", "", "16", height, width));

  // YUV
  // v.push_back(create_args("j2k", "444", "interleaved", "8", height, width));
  // v.push_back(create_args("j2k", "422", "interleaved", "8", height, width));
  // v.push_back(create_args("j2k", "420", "interleaved", "8", height, width));
  v.push_back(create_args("j2k", "444", "planar", "8", height, width));
  v.push_back(create_args("j2k", "422", "planar", "8", height, width));
  v.push_back(create_args("j2k", "420", "planar", "8", height, width));
}

// Sequences

void MainArgsGenerator::add_args_sequence_hevc(vector<MainArgs> &v) {
  MainArgs args;

  args.set_action("create_sequence");
  args.set_codec("hevc");

  // HEVC RGB Interleaved
  args.set_interleave("interleaved").set_chroma("rgb");
  v.push_back(args.set_pixel_type("8").set_output_filename());
  v.push_back(args.set_pixel_type("10").set_output_filename());
  v.push_back(args.set_pixel_type("12").set_output_filename());
  // v.push_back(args.set_pixel_type("14").set_output_filename());
  // v.push_back(args.set_pixel_type("16").set_output_filename());

  // HEVC RGB Planar
  args.set_interleave("planar").set_chroma("rgb");
  v.push_back(args.set_pixel_type("8").set_output_filename());
  // v.push_back(args.set_pixel_type("10").set_output_filename());
  // v.push_back(args.set_pixel_type("12").set_output_filename());
  // v.push_back(args.set_pixel_type("14").set_output_filename());
  // v.push_back(args.set_pixel_type("16").set_output_filename());

  // HEVC Monochrome
  args.set_chroma("mono");
  v.push_back(args.set_pixel_type("8").set_output_filename());
  // v.push_back(args.set_pixel_type("10").set_output_filename());
  // v.push_back(args.set_pixel_type("12").set_output_filename());
  // v.push_back(args.set_pixel_type("14").set_output_filename());
  // v.push_back(args.set_pixel_type("16").set_output_filename());

  // HEVC YUV
  // args.set_interleave("interleaved").set_pixel_type("8");
  // v.push_back(args.set_chroma("444").set_output_filename());
  // v.push_back(args.set_chroma("422").set_output_filename());
  // v.push_back(args.set_chroma("420").set_output_filename());
  args.set_interleave("planar").set_pixel_type("8");
  v.push_back(args.set_chroma("444").set_output_filename());
  v.push_back(args.set_chroma("422").set_output_filename());
  v.push_back(args.set_chroma("420").set_output_filename());
}

void MainArgsGenerator::add_args_sequence_uncompressed(vector<MainArgs> &v) {
  MainArgs args;
  args.set_action("create_sequence");

  args.set_codec("unc")
      .set_chroma("mono")
      .set_output_filename();
  v.push_back(args);
}

void MainArgsGenerator::add_args_sequence_av1(vector<MainArgs> &v) {
  MainArgs args;
  args.set_action("create_sequence");

  args.set_codec("av1")
      .set_pixel_type("8")
      .set_output_filename();
  v.push_back(args);
}

void MainArgsGenerator::add_args_sequence_j2k(vector<MainArgs> &v) {
  MainArgs args;
  args.set_action("create_sequence");

  args.set_codec("j2k").set_interleave("interleaved").set_chroma("rgb");
  v.push_back(args.set_pixel_type("8").set_output_filename());
  v.push_back(args.set_pixel_type("10").set_output_filename());
  v.push_back(args.set_pixel_type("12").set_output_filename());
  v.push_back(args.set_pixel_type("14").set_output_filename());
  v.push_back(args.set_pixel_type("16").set_output_filename());
}

// Debug

void MainArgsGenerator::debug(vector<MainArgs> &v) {
  add_args_simple_j2k(v);
}