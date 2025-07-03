#pragma once
#include "main_args.h"
#include <string>
#include <vector>

using namespace std;

namespace gimi {

class SampleFileGenerator {
  // TODO: move to inside Raw2Gimi
public:
  // static void generate_sample_files(const string &output_directory);

public:
  static const vector<MainArgs> simulate_cli_simple();
  static const vector<MainArgs> simulate_cli_grid();
  static const vector<MainArgs> simulate_cli_sequence();
  static string create_filename(MainArgs args);
  static MainArgs create_args(string encoding, string chroma, string interleave, string pixel_type, string width, string height);
};
} // namespace gimi