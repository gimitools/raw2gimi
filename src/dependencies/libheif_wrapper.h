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
  // Constructor
  LibheifWrapper(WriteOptions);
  // TODO: a heif file can have multiple images with varying write options.

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
  // heif_context *ctx; // TODO
};

// enum heif_chroma
// {
//   heif_chroma_undefined = 99,
//   heif_chroma_monochrome = 0,
//   heif_chroma_420 = 1,
//   heif_chroma_422 = 2,
//   heif_chroma_444 = 3,
//   heif_chroma_interleaved_RGB = 10,
//   heif_chroma_interleaved_RGBA = 11,
//   heif_chroma_interleaved_RRGGBB_BE = 12,   // HDR, big endian.
//   heif_chroma_interleaved_RRGGBBAA_BE = 13, // HDR, big endian.
//   heif_chroma_interleaved_RRGGBB_LE = 14,   // HDR, little endian.
//   heif_chroma_interleaved_RRGGBBAA_LE = 15  // HDR, little endian.
// };
