#include "correspondence_group.h"

using namespace ido;

// Constructors
CorrespondenceGroup::CorrespondenceGroup(const CornerPoints &corners, const BoundingBox &bbox) {
  Correspondence top_left(bbox.get_top_left(), corners.top_left);
  Correspondence top_right(bbox.get_top_right(), corners.top_right);
  Correspondence bottom_left(bbox.get_bottom_left(), corners.bottom_left);
  Correspondence bottom_right(bbox.get_bottom_right(), corners.bottom_right);

  correspondences.push_back(top_left);
  correspondences.push_back(top_right);
  correspondences.push_back(bottom_left);
  correspondences.push_back(bottom_right);
}

// Getters
vector<Coordinate> CorrespondenceGroup::get_coordinates() const {
  vector<Coordinate> cords;
  for (const auto &correspondence : correspondences) {
    cords.push_back(correspondence.coordinate);
  }
  return cords;
}