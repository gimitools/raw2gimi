#include "raw_image_grid.h"
#include "error_handler.h"

using namespace gimi;

void RawImageGrid::add_row(const vector<RawImage> &row) {
  m_tiles.push_back(row);
}

uint32_t RawImageGrid::get_row_count() const {
  return m_tiles.size();
}

uint32_t RawImageGrid::get_column_count() const {
  if (m_tiles.empty()) {
    return 0;
  }
  return m_tiles[0].size();
}

uint32_t RawImageGrid::get_tile_width() const {
  if (m_tiles.empty() || m_tiles[0].empty()) {
    return 0;
  }
  return m_tiles[0][0].get_width();
}

uint32_t RawImageGrid::get_tile_height() const {
  if (m_tiles.empty() || m_tiles[0].empty()) {
    return 0;
  }
  return m_tiles[0][0].get_height();
}

RawImage RawImageGrid::get_tile(uint32_t row, uint32_t column) const {
  if (row >= get_row_count() || column >= get_column_count()) {
    throw_error("Tile index out of bounds: (%d, %d)", row, column);
  }
  return m_tiles[row][column];
}

vector<vector<RawImage>> RawImageGrid::get_tiles() const {
  return m_tiles;
}