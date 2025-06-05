#include "pixel_formats.h"
#include "error_handler.h"

using namespace gimi;

// To String

string gimi::to_string(PixelType pixel_type) {
  switch (pixel_type) {
  case PixelType::uint8:
    return "uint8";
  case PixelType::uint10:
    return "uint10";
  case PixelType::uint12:
    return "uint12";
  case PixelType::uint14:
    return "uint14";
  case PixelType::uint16:
    return "uint16";
  case PixelType::int8:
    return "int8";
  case PixelType::int16:
    return "int16";
  case PixelType::float32:
    return "float32";
  case PixelType::complex:
    return "complex";
  case PixelType::mixed:
    return "mixed";
  }
  return "unknown_bit_depth";
}

string gimi::to_string(Chroma chroma) {
  switch (chroma) {
  case Chroma::rgb:
    return "rgb";
  case Chroma::rgba:
    return "rgba";
  case Chroma::bayer:
    return "bayer";
  case Chroma::gray:
    return "gray";
  case Chroma::yuv_444:
    return "yuv_444";
  case Chroma::yuv_422:
    return "yuv_422";
  case Chroma::yuv_420:
    return "yuv_420";
  case Chroma::yuv_411:
    return "yuv_411";
  }
  return "unknown_chroma";
}

string gimi::to_string(Interleave interleave) {
  switch (interleave) {
  case Interleave::planar:
    return "planar";
  case Interleave::interleaved:
    return "interleaved";
    // case Interleave::mixed:
    //   return "mixed";
    // case Interleave::row:
    //   return "row";
    // case Interleave::tile_component:
    //   return "tile_component";
    // case Interleave::multi_y_pixel:
    //   return "multi_y_pixel";
  }
  return "unknown_interleave";
}

string gimi::to_string(Codec codec) {
  switch (codec) {
  case Codec::raw:
    return "raw";
  case Codec::hevc:
    return "hevc";
  case Codec::avc:
    return "avc";
  case Codec::vvc:
    return "vvc";
  case Codec::av1:
    return "av1";
  case Codec::jpeg:
    return "jpeg";
  case Codec::j2k:
    return "j2k";
  case Codec::htj2k:
    return "htj2k";
  }
  return "unknown_codec";
}

// Getters

uint32_t gimi::get_bit_depth(PixelType pixel_type) {
  switch (pixel_type) {
  case PixelType::uint8:
    return 8;
  case PixelType::uint10:
    return 10;
  case PixelType::uint12:
    return 12;
  case PixelType::uint14:
    return 14;
  case PixelType::uint16:
    return 16;
  case PixelType::int8:
    return 8;
  case PixelType::int16:
    return 16;
  case PixelType::float32:
    return 32;
  case PixelType::complex:
    return 64; // Assuming complex is represented as two float32 values
  case PixelType::mixed:
    return 0; // Mixed bit depth, not a single value
  }
  return 0; // Default for unknown bit depth
}

uint32_t gimi::get_component_count(Chroma chroma) {
  switch (chroma) {
  case Chroma::gray:
    return 1;
  case Chroma::bayer:
  case Chroma::rgb:
    return 3;
  case Chroma::yuv_444:
  case Chroma::yuv_422:
  case Chroma::yuv_420:
  case Chroma::yuv_411:
    return 3;
  case Chroma::rgba:
    return 4;
  }
  throw_error("Unsupported chroma format: %s", to_string(chroma));
  return 0;
}