#pragma once

#include "main_args.h"
using namespace std;

namespace gimi {
class MainArgsBuilder {
public:
  // Constructor
  MainArgsBuilder() = default;

public:
  // Build Function
  MainArgs build();

public:
  MainArgsBuilder &input_filename(const string &filename);
  MainArgsBuilder &output_filename(const string &filename);
  MainArgsBuilder &codec(const string &codec);
  MainArgsBuilder &chroma(const string &chroma);
  MainArgsBuilder &interleave(const string &interleave);
  MainArgsBuilder &pixel_type(const string &pixel_type);
  MainArgsBuilder &action(const string &action);
  MainArgsBuilder &debug(const string &debug);
  MainArgsBuilder &pixel_algorithm(const string &pixel_algorithm);
  MainArgsBuilder &width(const string &width);
  MainArgsBuilder &height(const string &height);
  MainArgsBuilder &rows(const string &rows);
  MainArgsBuilder &columns(const string &columns);
  MainArgsBuilder &tile_directory(const string &tile_directory);
  MainArgsBuilder &scale_factor(const string &scale_factor);

private:
  MainArgs args;
};
} // namespace gimi