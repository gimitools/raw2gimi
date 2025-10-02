#include "bounding_box.h"
#include <sstream>

using namespace gimi;

// Constructors

BoundingBox::BoundingBox(const Coordinate &top_left,
                         const Coordinate &top_right,
                         const Coordinate &bottom_left,
                         const Coordinate &bottom_right) : m_top_left(top_left),
                                                           m_top_right(top_right),
                                                           m_bottom_left(bottom_left),
                                                           m_bottom_right(bottom_right) {
}

// Getters

Coordinate BoundingBox::get_top_left() const {
  return m_top_left;
}

Coordinate BoundingBox::get_top_right() const {
  return m_top_right;
}

Coordinate BoundingBox::get_bottom_left() const {
  return m_bottom_left;
}

Coordinate BoundingBox::get_bottom_right() const {
  return m_bottom_right;
}

string BoundingBox::to_string() const {
  std::ostringstream oss;
  oss << "BoundingBox(TopLeft: (" << m_top_left.get_latitude() << ", " << m_top_left.get_longitude() << "), "
      << "TopRight: (" << m_top_right.get_latitude() << ", " << m_top_right.get_longitude() << "), "
      << "BottomLeft: (" << m_bottom_left.get_latitude() << ", " << m_bottom_left.get_longitude() << "), "
      << "BottomRight: (" << m_bottom_right.get_latitude() << ", " << m_bottom_right.get_longitude() << "))";
  return oss.str();
}