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
  args.action = "create_image";
  args.codec = encoding;
  args.chroma = chroma;
  args.interleave = interleave;
  args.pixel_type = pixel_type;
  args.width = width;
  args.height = height;
  args.output_filename = create_filename(args);
  return args;
}

void MainArgsGenerator::add_args_simple(vector<MainArgs> &v) {
  string codec = "hevc";
  string chroma = "rgb";
  string interleave = "interleaved";
  string pixel_type = "8";
  string width = "64";
  string height = "64";

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

  // Tiled
  // 1. grid
  // 2. uncC internal
  // 3. tili
};

void MainArgsGenerator::add_args_grid(vector<MainArgs> &v) {
  MainArgs grid_2x2;
  {
    grid_2x2.action = "create_grid";
    grid_2x2.rows = "2";
    grid_2x2.columns = "2";
    grid_2x2.output_filename = "out/hevc_grid_2x2.heif";
    grid_2x2.codec = "hevc";
    v.push_back(grid_2x2);
  }

  MainArgs grid_large;
  {
    grid_large.action = "create_grid";
    grid_large.rows = "10";
    grid_large.columns = "10";
    grid_large.output_filename = "out/hevc_grid_10x10.heif";
    grid_large.codec = "hevc";
    v.push_back(grid_large);
  }

  // MainArgs uncC_internal;
  // MainArgs tili_item;
}

void MainArgsGenerator::add_args_sequence(vector<MainArgs> &v) {
  MainArgs args;
  MainArgsBuilder builder;
  // Defaults
  builder.action("create_sequence")
      .chroma("rgb")
      .interleave("planar")
      .pixel_type("8");

  args = builder.codec("hevc")
             .output_filename("out/hevc_sequence_64x64.mp4")
             .build();
  v.push_back(args);

  args = builder.codec("av1")
             .output_filename("out/av1_sequence_64x64.mp4")
             .build();
  v.push_back(args);
}

string MainArgsGenerator::create_filename(MainArgs p) {
  // TODO: move to Raw2Gimi constructor
  string filename = "out/" +
                    p.codec + "_" +
                    p.chroma + "_" +
                    p.interleave + "_" +
                    p.pixel_type + "bit_" +
                    p.width + "x" +
                    p.height + ".heif";
  return filename;
}