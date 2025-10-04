#pragma once

#include "model/point.h"

class CornerPoints {
public:
  // Constructors
  CornerPoints(const Point &top_left, const Point &top_right, const Point &bottom_left,
               const Point &bottom_right);

public:
  Point top_left;
  Point top_right;
  Point bottom_left;
  Point bottom_right;
};