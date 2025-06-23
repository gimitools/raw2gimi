#pragma once

#include "main_args.h"
#include "model/write_options.h"

namespace gimi {

class Raw2Gimi {
public:
  // Constructor
  Raw2Gimi(MainArgs);

public:
  // CLI API
  void write_image();
  void write_grid();
  void write_sequence();
  void raw_to_gimi();
  void heif_to_gimi();
  void write_image_with_rdf();

protected:
  // Primary Functions
  static void raw_to_gimi(const string &input_filename, const string &output_filename);

private:
  // Helper Functions
  WriteOptions create_write_options();

private:
  // Member Variables
  uint32_t m_width;
  uint32_t m_height;
  uint32_t m_rows;
  uint32_t m_columns;
  PixelType m_pixel_type;
  Chroma m_chroma;
  Interleave m_interleave;
  Codec m_codec;
  string m_input_filename;
  string m_output_filename;
};

} // namespace gimi