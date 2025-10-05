#include "timestamp.h"

// Constructor
Timestamp::Timestamp(uint64_t tai) : tai(tai) {
  set_label("TAI Timestamp");
}

// Getters
uint64_t Timestamp::get_tai() const {
  return tai;
}