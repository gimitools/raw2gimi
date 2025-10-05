#include "correspondence_group.h"

using namespace ido;

// Constructors
CorrespondenceGroup::CorrespondenceGroup(const vector<Correspondence> &correspondences)
    : correspondences(correspondences) {

  set_label("Correspondence Group");
}

// Getters
vector<Coordinate> CorrespondenceGroup::get_coordinates() const {
  vector<Coordinate> cords;
  for (const auto &correspondence : correspondences) {
    cords.push_back(correspondence.coordinate);
  }
  return cords;
}