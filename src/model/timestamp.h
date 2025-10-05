#pragma once

#include "model/resource.h"
#include <cstdint>

class Timestamp : public Resource {

public:
  // Constructor
  Timestamp(uint64_t tai);

public:
  // Getters
  uint64_t get_tai() const;

private:
  uint64_t tai; // TAI time in nanoseconds since epoch
};