#pragma once

#include "main_args.h"

class Raw2Gimi {
public:
  // CLI API
  static void write_image_from_memory(MainArgs);
  static void raw_to_gimi(MainArgs args);
  static void heif_to_gimi(MainArgs args);

protected:
  // Primary Functions
  static heif_context *encode_image_from_memory(heif_compression_format, uint32_t width, uint32_t height, heif_chroma, heif_colorspace, uint32_t bit_depth);
  static void raw_to_gimi(const string &input_filename, const string &output_filename);

protected:
  // Helper Functions
  static void he(struct heif_error error);
};