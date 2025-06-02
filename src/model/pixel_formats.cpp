#include "pixel_formats.h"

using namespace gimi;

string to_string(BitDepth bit_depth) {
  switch (bit_depth) {
  case BitDepth::uint8:
    return "uint8";
  case BitDepth::uint10:
    return "uint10";
  case BitDepth::uint12:
    return "uint12";
  case BitDepth::uint14:
    return "uint14";
  case BitDepth::uint16:
    return "uint16";
  case BitDepth::int8:
    return "int8";
  case BitDepth::int16:
    return "int16";
  case BitDepth::float32:
    return "float32";
  case BitDepth::complex:
    return "complex";
  case BitDepth::mixed:
    return "mixed";
  }
  return "unknown_bit_depth";
}

string to_string(Chroma chroma) {
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

string to_string(Interleave interleave) {
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

string to_string(Codec codec) {
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
