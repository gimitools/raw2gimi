#pragma once

#include "model/coordinate.h"
#include "model/point.h"
#include "model/resource.h"

using namespace gimi;
using namespace std;

struct Correspondence : public Resource {
public:
  // Constructors
  Correspondence(const Coordinate &, const Point &);

public:
  Coordinate coordinate;
  Point point;
};