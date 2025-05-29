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
  static void raw_to_gimi(const string &input_filename, const string &output_filename);
};