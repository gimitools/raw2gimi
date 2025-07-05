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
  void add_image(const RawImage &);
  void add_grid(const RawImageGrid &);
  void add_video(const vector<RawImage> &);
  void add_metadata_track();
  void write_to_heif();

protected:
  // GIMI
  void gimify(heif_item_id primary_id);
  void add_content_id(heif_item_id);
  void add_timestamp(heif_item_id);

protected:
  // Helper Functions
  heif_image *convert_to_heif_image(const RawImage &, heif_colorspace, heif_chroma);
  heif_image *convert_yuv_colorspace(const RawImage &, heif_chroma);
  heif_image *convert_rgb_colorspace(const RawImage &, heif_chroma);
  heif_image *convert_gray_colorspace(const RawImage &, heif_chroma);

protected:
  // Leaf Functions
  heif_image *convert_rgb_planar(const RawImage &);
  heif_image *convert_rgb_interleaved(const RawImage &);
  heif_image *convert_rgb_interleaved_hdr_be(const RawImage &);
  heif_image *convert_yuv_444_planar_8bit(const RawImage &);

protected:
  // Static Functions
  static void he(struct heif_error);
  static heif_compression_format extract_compression(gimi::Codec);
  static heif_colorspace extract_colorspace(const RawImage &);
  static heif_chroma extract_chroma(const RawImage &);
  static string generate_content_id();

private:
  // Member Variables
  WriteOptions m_options;
  heif_context *m_ctx;
};
