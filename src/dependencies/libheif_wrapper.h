#pragma once
#include "model/pixel_formats.h"
#include "model/raw_image.h"
#include "model/write_options.h"
#include <libheif/heif.h>
#include <string>
using namespace std;
using namespace gimi;

class LibheifWrapper {

public:
  // Constructors
  LibheifWrapper(WriteOptions); // TODO: a heif file can have multiple images with varying write options.
  ~LibheifWrapper();

public:
  // API
  void add_image(const RawImage &image);
  void write_to_heif();

protected:
  // Helper Functions
  static void he(struct heif_error);
  static heif_compression_format extract_compression(gimi::Codec);
  static heif_colorspace extract_colorspace(gimi::Chroma, gimi::Interleave);
  static heif_chroma extract_chroma(const RawImage &image, WriteOptions);

private:
  // Member Variables
  WriteOptions m_options;
  heif_context *m_ctx; // TODO
};
