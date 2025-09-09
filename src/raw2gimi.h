#pragma once

#include "main_args.h"
#include "model/write_options.h"

namespace gimi {

  class Raw2Gimi {
  public:
    // Constructor
    Raw2Gimi(MainArgs);

  public:
    // Primary API Function
    void execute_action();

  public:
    // CLI API
    void create_image();
    void create_grid();
    void create_sequence();
    void raw_to_gimi();
    void heif_to_gimi();
    void write_image_with_rdf();
    void generate_sample_files();

  protected:
    // Primary Functions
    static void raw_to_gimi(const string &input_filename, const string &output_filename);
    static void heif_to_gimi(const string &input_filename, WriteOptions);

  private:
    // Helper Functions
    WriteOptions create_write_options();

  private:
    // Member Variables
    string m_action;
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