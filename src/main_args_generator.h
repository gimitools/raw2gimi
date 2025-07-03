#pragma once
#include "builders/main_args_builder.h"
#include "main_args.h"
#include <string>
#include <vector>

using namespace std;

namespace gimi {

class MainArgsGenerator {
  // TODO: move to inside Raw2Gimi
public:
  static const vector<MainArgs> generate_main_args();

protected:
  static void simulate_cli_simple(vector<MainArgs> &);
  static void simulate_cli_grid(vector<MainArgs> &);
  static void simulate_cli_sequence(vector<MainArgs> &);
  static string create_filename(MainArgs args);
  static MainArgs create_args(string encoding, string chroma, string interleave, string pixel_type, string width, string height);
};
} // namespace gimi