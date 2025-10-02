#include "image_coordinate.h"

// Constructors
ImageCoordinate::ImageCoordinate(uint32_t x, uint32_t y) : m_x(x), m_y(y) {}

// Getters

uint32_t ImageCoordinate::get_x() const {
  return m_x;
}

uint32_t ImageCoordinate::get_y() const {
  return m_y;
}