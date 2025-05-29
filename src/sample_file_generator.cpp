#include "sample_file_generator.h"

using namespace gimi;

void SampleFileGenerator::generate_sample_files(const string &output_directory) {
  vector<MainArgs> data = SampleFileGenerator::create_cli_args();
  for (const MainArgs &args : data) {
    Raw2Gimi::write_image_from_memory(args);
  }
}

MainArgs SampleFileGenerator::create_args(string encoding, string chroma, string interleave, string bit_depth, string width, string height) {
  MainArgs args;
  args.codec = encoding;
  args.chroma = chroma;
  args.interleave = interleave;
  args.bit_depth = bit_depth;
  args.width = width;
  args.height = height;
  args.output_filename = create_filename(args);
  return args;
}

const vector<MainArgs> SampleFileGenerator::create_cli_args() {
  vector<MainArgs> v;
  string codec = "unc";
  string chroma = "rgb";
  string interleave = "interleaved";
  string bit_depth = "8";
  string width = "64";
  string height = "64";

  // Default
  v.push_back(create_args(codec, chroma, interleave, bit_depth, height, width));

  // Codec variations
  // v.push_back(create_args("unc", chroma, interleave, bit_depth, height, width));
  // v.push_back(create_args("j2k", chroma, interleave, bit_depth, height, width));
  // v.push_back(create_args("hevc", chroma, interleave, bit_depth, height, width));
  // v.push_back(create_args("av1", chroma, interleave, bit_depth, height, width));

  // HDR
  // v.push_back(create_args(codec, chroma, interleave, "10", height, width));
  // v.push_back(create_args(codec, chroma, interleave, "12", height, width));
  // v.push_back(create_args(codec, chroma, interleave, "14", height, width));
  // v.push_back(create_args(codec, chroma, interleave, "16", height, width));

  // Formats variations
  // v.push_back(create_args(codec, "rgb", "rgb", bit_depth, height, width));
  // v.push_back(create_args(codec, "rgb", "planar", bit_depth, height, width));
  // v.push_back(create_args(codec, "yuv", "444", bit_depth, height, width));
  // v.push_back(create_args(codec, "yuv", "422", bit_depth, height, width));
  // v.push_back(create_args(codec, "yuv", "422", bit_depth, height, width));

  return v;
};

string SampleFileGenerator::create_filename(MainArgs args) {

  string interleave = "interleave"; // TODO - read args.interleave enum
  string chroma = "chroma";         // TODO - read args.chroma enum

  string filename = "out/" + args.codec + "_" + "todointerleave" + "_" + "todochroma" + "_" +
                    args.bit_depth + "bit_" + args.width + "x" + args.height + ".heif";
  return filename;
}