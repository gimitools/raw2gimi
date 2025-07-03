#include "main_args_builder.h"

using namespace gimi;

MainArgs MainArgsBuilder::build() {
  return args;
}

MainArgsBuilder &MainArgsBuilder::input_filename(const string &input_filename) {
  args.input_filename = input_filename;
  return *this;
}

MainArgsBuilder &MainArgsBuilder::output_filename(const string &output_filename) {
  args.output_filename = output_filename;
  return *this;
}

MainArgsBuilder &MainArgsBuilder::codec(const string &codec) {
  args.codec = codec;
  return *this;
}

MainArgsBuilder &MainArgsBuilder::chroma(const string &chroma) {
  args.chroma = chroma;
  return *this;
}

MainArgsBuilder &MainArgsBuilder::interleave(const string &interleave) {
  args.interleave = interleave;
  return *this;
}

MainArgsBuilder &MainArgsBuilder::pixel_type(const string &pixel_type) {
  args.pixel_type = pixel_type;
  return *this;
}

MainArgsBuilder &MainArgsBuilder::action(const string &action) {
  args.action = action;
  return *this;
}

MainArgsBuilder &MainArgsBuilder::debug(const string &debug) {
  args.debug = debug;
  return *this;
}

MainArgsBuilder &MainArgsBuilder::pixel_algorithm(const string &pixel_algorithm) {
  args.pixel_algorithm = pixel_algorithm;
  return *this;
}

MainArgsBuilder &MainArgsBuilder::width(const string &width) {
  args.width = width;
  return *this;
}

MainArgsBuilder &MainArgsBuilder::height(const string &height) {
  args.height = height;
  return *this;
}

MainArgsBuilder &MainArgsBuilder::rows(const string &rows) {
  args.rows = rows;
  return *this;
}

MainArgsBuilder &MainArgsBuilder::columns(const string &columns) {
  args.columns = columns;
  return *this;
}

MainArgsBuilder &MainArgsBuilder::tile_directory(const string &tile_directory) {
  args.tile_directory = tile_directory;
  return *this;
}

MainArgsBuilder &MainArgsBuilder::scale_factor(const string &scale_factor) {
  args.scale_factor = scale_factor;
  return *this;
}