#pragma once
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
  static void add_args_simple(vector<MainArgs> &);
  static void add_args_grid(vector<MainArgs> &);
  static void add_args_sequence(vector<MainArgs> &);
  static string create_filename(MainArgs args);
  static MainArgs create_args(string encoding, string chroma, string interleave, string pixel_type, string width, string height);

protected:
  static void add_args_simple_uncompressed(vector<MainArgs> &);
};
} // namespace gimi