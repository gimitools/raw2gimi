#include "sample_file_generator.h"

using namespace gimi;

void SampleFileGenerator::generate_sample_files(const string &output_directory) {
  vector<MainArgs> simple_args = SampleFileGenerator::simulate_cli_simple();
  for (const MainArgs &args : simple_args) {
    Raw2Gimi::write_image_from_memory(args);
  }

  vector<MainArgs> grid_args = SampleFileGenerator::simulate_cli_grid();
  for (const MainArgs &args : grid_args) {
    Raw2Gimi::write_grid_from_memory(args);
  }
}

MainArgs SampleFileGenerator::create_args(string encoding, string chroma, string interleave, string pixel_type, string width, string height) {
  MainArgs args;
  args.codec = encoding;
  args.chroma = chroma;
  args.interleave = interleave;
  args.pixel_type = pixel_type;
  args.width = width;
  args.height = height;
  args.output_filename = create_filename(args);
  return args;
}

const vector<MainArgs> SampleFileGenerator::simulate_cli_simple() {
  vector<MainArgs> v;
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
  // v.push_back(create_args("unc", chroma, interleave, pixel_type, height, width));
  // v.push_back(create_args("hevc", chroma, interleave, pixel_type, height, width));
  // v.push_back(create_args("av1", chroma, interleave, pixel_type, height, width));
  // Warning! OpenJPEG not compiled in!
  // v.push_back(create_args("j2k", chroma, interleave, pixel_type, height, width));

  // HDR
  // v.push_back(create_args("unc", chroma, interleave, "10", height, width));
  // v.push_back(create_args("unc", chroma, interleave, "12", height, width));
  // v.push_back(create_args("unc", chroma, interleave, "14", height, width));
  // v.push_back(create_args("unc", chroma, interleave, "16", height, width));

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

  return v;
};

const vector<MainArgs> SampleFileGenerator::simulate_cli_grid() {
  vector<MainArgs> v;

  MainArgs grid_2x2;
  {
    grid_2x2.action = "write_grid_from_memory";
    grid_2x2.rows = "2";
    grid_2x2.columns = "2";
    grid_2x2.output_filename = "out/hevc_grid_2x2.heif";
    grid_2x2.codec = "hevc";
    v.push_back(grid_2x2);
  }

  MainArgs grid_large;
  {
    grid_large.action = "write_grid_from_memory";
    grid_large.rows = "200";
    grid_large.columns = "200";
    grid_large.output_filename = "out/hevc_grid_200x200.heif";
    grid_large.codec = "hevc";
    v.push_back(grid_large);
  }

  // MainArgs uncC_internal;
  // MainArgs tili_item;

  return v;
}

string SampleFileGenerator::create_filename(MainArgs p) {
  string filename = "out/" +
                    p.codec + "_" +
                    p.chroma + "_" +
                    p.interleave + "_" +
                    p.pixel_type + "bit_" +
                    p.width + "x" +
                    p.height + ".heif";
  return filename;
}