#pragma once

#include "model/bounding_box.h"
#include "model/corner_points.h"
#include "model/correspondence.h"
#include "model/resource.h"
#include <vector>

using namespace std;

namespace ido {

  struct CorrespondenceGroup : public Resource {
  public:
    // Constructors
    CorrespondenceGroup(const vector<Correspondence> &);

  public:
    // Getters
    vector<Coordinate> get_coordinates() const;

  public:
    // Member Variables
    vector<Correspondence> correspondences;
  };

} // namespace ido