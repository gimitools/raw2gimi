#include "point.h"

// Constructors
Point::Point(uint32_t x, uint32_t y) : m_x(x), m_y(y) {
  set_label("Image Coordinate");
}

// Getters

uint32_t Point::get_x() const {
  return m_x;
}

uint32_t Point::get_y() const {
  return m_y;
}