#include "sample_file_generator.h"

using namespace gimi;

void SampleFileGenerator::generate_sample_files(const string &output_directory) {
  vector<MainArgs> data = SampleFileGenerator::desired_data();
  for (const MainArgs &args : data) {
    Raw2Gimi::write_image_from_memory(args);
  }
}

MainArgs SampleFileGenerator::create_args(string encoding, string colorspace, string chroma, string bit_depth, string width, string height) {
  MainArgs args;
  args.encoding = encoding;
  args.colorspace = colorspace;
  args.chroma = chroma;
  args.bit_depth = bit_depth;
  args.width = width;
  args.height = height;
  args.output_filename = create_filename(args);
  return args;
}

const vector<MainArgs> SampleFileGenerator::desired_data() {
  vector<MainArgs> v;
  string codec = "unc";
  string colorspace = "rgb";
  string chroma = "rgb";
  string bit_depth = "8";
  string width = "64";
  string height = "64";

  // Default
  v.push_back(create_args(codec, colorspace, chroma, bit_depth, height, width));

  // Codec variations
  // v.push_back(create_args("unc", colorspace, chroma, bit_depth, height, width));
  // v.push_back(create_args("j2k", colorspace, chroma, bit_depth, height, width));
  // v.push_back(create_args("hevc", colorspace, chroma, bit_depth, height, width));
  // v.push_back(create_args("av1", colorspace, chroma, bit_depth, height, width));

  // Bit Depth
  // v.push_back(create_args(codec, colorspace, chroma, "10", height, width));
  // v.push_back(create_args(codec, colorspace, chroma, "12", height, width));
  // v.push_back(create_args(codec, colorspace, chroma, "14", height, width));
  // v.push_back(create_args(codec, colorspace, chroma, "16", height, width));

  // Formats variations
  v.push_back(create_args(codec, "rgb", "rgb", bit_depth, height, width));
  v.push_back(create_args(codec, "rgb", "planar", bit_depth, height, width));
  v.push_back(create_args(codec, "yuv", "444", bit_depth, height, width));
  v.push_back(create_args(codec, "yuv", "422", bit_depth, height, width));
  // v.push_back(create_args(codec, "yuv", "422", bit_depth, height, width));

  return v;
};

string SampleFileGenerator::create_filename(MainArgs args) {
  /*
  colorspace - rgb, yuv, monochrome
  chroma

  */
  string chroma = args.chroma;
  if (chroma == "rgb") {
    chroma = "interleaved"; // The colorspace is already RGB
  }

  string filename = "out/" + args.encoding + "_" + args.colorspace + "_" + chroma + "_" +
                    args.bit_depth + "bit_" + args.width + "x" + args.height + ".heif";
  return filename;
}