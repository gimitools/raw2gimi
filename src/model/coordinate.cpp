#include "coordinate.h"

using namespace gimi;

// Constructors

Coordinate::Coordinate(double latitude, double longitude) {
  m_latitude = latitude;
  m_longitude = longitude;
}

// Getters
double Coordinate::get_latitude() const {
  return m_latitude;
}

double Coordinate::get_longitude() const {
  return m_longitude;
}