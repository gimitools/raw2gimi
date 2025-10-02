#pragma once

#include <iostream>

using namespace std;

namespace gimi {
  class Coordinate {
  public:
    // Constructors
    Coordinate(double latitude, double longitude);

  public:
    // Getters
    double get_latitude() const;
    double get_longitude() const;

  public:
    string to_string() const;

  private:
    double m_latitude;
    double m_longitude;
  };
} // namespace gimi