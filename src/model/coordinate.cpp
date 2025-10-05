#include "coordinate.h"
#include <cmath> // std::fabs

using namespace gimi;
using namespace std;

// Constructors

Coordinate::Coordinate(double latitude, double longitude) {
  m_latitude = latitude;
  m_longitude = longitude;
  set_label("Ground Coordinate");
}

// Getters
double Coordinate::get_latitude() const {
  return m_latitude;
}

double Coordinate::get_longitude() const {
  return m_longitude;
}

string Coordinate::to_string() const {
  return "(" + std::to_string(m_latitude) + ", " + std::to_string(m_longitude) + ")";
}

bool Coordinate::operator==(const Coordinate &other) const {
  double EPS = 1e-12;
  return std::fabs(m_latitude - other.m_latitude) < EPS &&
         std::fabs(m_longitude - other.m_longitude) < EPS;
}

bool Coordinate::operator!=(const Coordinate &other) const {
  return !(*this == other);
}