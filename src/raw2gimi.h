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
    void create_image(WriteOptions);
    void create_grid(WriteOptions);
    void create_sequence(WriteOptions);
    void image_to_gimi(WriteOptions);
    void raw_to_gimi(WriteOptions);
    void heif_to_gimi(WriteOptions); // Simple heif to gimi. uniform tiles. no sequence. single image.
    void tiles_to_grid(WriteOptions);
    void image_to_tiles(WriteOptions);
    void write_image_with_rdf(WriteOptions);
    void generate_sample_files(WriteOptions);

  protected:
    // Primary Functions
    static void raw_to_gimi(const string &input_filename, const string &output_filename);
    static void heif_to_gimi(const string &input_filename, WriteOptions);

  private:
    // Helper Functions
    WriteOptions create_write_options();

  private:
    // Member Variables
    MainArgsAction m_action;
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
    string m_image_name;
    string m_sidecar_filename;
    MainArgsSidecarType m_sidecar_type;
  };

} // namespace gimi