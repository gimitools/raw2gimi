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
  static void write_to_heif(const RawImage &image, WriteOptions);

private:
  static void he(struct heif_error);
  static heif_compression_format extract_compression(gimi::Codec);
  static heif_colorspace extract_colorspace(gimi::Chroma, gimi::Interleave);
  static heif_chroma extract_chroma(WriteOptions);
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

// enum heif_colorspace {
//   heif_colorspace_undefined = 99,

//   heif_colorspace_YCbCr = 0,

//   // * heif_chroma_444 (for planar RGB)
//   heif_colorspace_RGB = 1,

//   heif_colorspace_monochrome = 2,

//   heif_colorspace_nonvisual = 3
// };