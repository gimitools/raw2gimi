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
  static MainArgs create_args(string encoding, string chroma, string interleave, string pixel_type, string width, string height);

protected:
  // Simple Images
  static void add_args_simple_uncompressed(vector<MainArgs> &);
  static void add_args_simple_hevc(vector<MainArgs> &);
  static void add_args_simple_av1(vector<MainArgs> &);
  static void add_args_simple_j2k(vector<MainArgs> &);

protected:
  // Sequences
  static void add_args_sequence_hevc(vector<MainArgs> &);
  static void add_args_sequence_uncompressed(vector<MainArgs> &);
  static void add_args_sequence_av1(vector<MainArgs> &);
  static void add_args_sequence_j2k(vector<MainArgs> &);

protected:
  // Debug
  static void debug(vector<MainArgs> &v);
};
} // namespace gimi