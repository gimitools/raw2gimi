#pragma once

#include <iostream>

#include "coordinate.h"

using namespace std;

namespace gimi {

  class BoundingBox {

  public:
    // Constructors
    BoundingBox(const Coordinate &top_left,
                const Coordinate &top_right,
                const Coordinate &bottom_left,
                const Coordinate &bottom_right);

  public:
    // Getters
    Coordinate get_top_left() const;
    Coordinate get_top_right() const;
    Coordinate get_bottom_left() const;
    Coordinate get_bottom_right() const;

  public:
    string to_string() const;

  private:
    // Member Variables
    Coordinate m_top_left;
    Coordinate m_top_right;
    Coordinate m_bottom_left;
    Coordinate m_bottom_right;
  };
} // namespace gimi