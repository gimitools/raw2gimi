#pragma once
#include "main_args.h"
#include "raw2gimi.h"
#include <string>
#include <vector>

using namespace std;

namespace gimi {

class SampleFileGenerator {
public:
  static void generate_sample_files(const string &output_directory);

protected:
  static const vector<MainArgs> desired_data();
  static string create_filename(MainArgs args);
  static MainArgs create_args(string encoding, string sampling, string interleave, string bit_depth, string width, string height);
};
} // namespace gimi