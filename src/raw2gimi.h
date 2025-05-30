#pragma once

#include "main_args.h"
#include "model/write_options.h"

namespace gimi {

class Raw2Gimi {
public:
  // CLI API
  static void write_image_from_memory(MainArgs);
  static void raw_to_gimi(MainArgs args);
  static void heif_to_gimi(MainArgs args);

protected:
  // Primary Functions
  static void raw_to_gimi(const string &input_filename, const string &output_filename);

private:
  // Helper Functions
  static WriteOptions create_write_options(MainArgs);
};

} // namespace gimi