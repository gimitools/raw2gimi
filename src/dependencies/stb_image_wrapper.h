#pragma once

#include "model/raw_image.h"
#include <cstdint>
#include <string>
#include <vector>

using namespace std;

class StbImageWrapper {
public:
  static gimi::RawImage load(const string &filename);
};
