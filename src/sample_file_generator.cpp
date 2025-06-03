#include "sample_file_generator.h"

using namespace gimi;

void SampleFileGenerator::generate_sample_files(const string &output_directory) {
  vector<MainArgs> data = SampleFileGenerator::simulate_user_cli();
  for (const MainArgs &args : data) {
    Raw2Gimi::write_image_from_memory(args);
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

const vector<MainArgs> SampleFileGenerator::simulate_user_cli() {
  vector<MainArgs> v;
  string codec = "unc";
  string chroma = "rgb";
  string interleave = "interleaved";
  string pixel_type = "8";
  string width = "64";
  string height = "64";

  // Default
  // v.push_back(create_args(codec, chroma, interleave, pixel_type, height, width));

  // Codec variations
  // v.push_back(create_args("unc", chroma, interleave, pixel_type, height, width));
  // v.push_back(create_args("hevc", chroma, interleave, pixel_type, height, width));
  // v.push_back(create_args("av1", chroma, interleave, pixel_type, height, width));
  // Warning! OpenJPEG not compiled in!
  // v.push_back(create_args("j2k", chroma, interleave, pixel_type, height, width));

  // HDR
  v.push_back(create_args(codec, chroma, interleave, "10", height, width));
  // v.push_back(create_args(codec, chroma, interleave, "12", height, width));
  // v.push_back(create_args(codec, chroma, interleave, "14", height, width));
  // v.push_back(create_args(codec, chroma, interleave, "16", height, width));

  // Formats variations
  // v.push_back(create_args(codec, "rgb", "rgb", pixel_type, height, width));
  // v.push_back(create_args(codec, "rgb", "planar", pixel_type, height, width));
  // v.push_back(create_args(codec, "yuv", "444", pixel_type, height, width));
  // v.push_back(create_args(codec, "yuv", "422", pixel_type, height, width));
  // v.push_back(create_args(codec, "yuv", "422", pixel_type, height, width));

  return v;
};

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