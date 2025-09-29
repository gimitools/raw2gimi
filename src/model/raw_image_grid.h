#pragma once

#include "model/raw_image.h"
#include <cstdint>
#include <iostream>
#include <vector>

using namespace std;

namespace gimi {

  class RawImageGrid {

  public:
    void add_row(const vector<RawImage> &row);
    uint32_t get_row_count() const;
    uint32_t get_column_count() const;
    uint32_t get_tile_width() const;
    uint32_t get_tile_height() const;
    RawImage get_tile(uint32_t row, uint32_t column) const;
    vector<vector<RawImage>> get_tiles() const;

  private:
    vector<vector<RawImage>> m_tiles;
  };

} // namespace gimi