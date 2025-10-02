#pragma once

#include "model/resource.h"
#include <stdint.h>

class ImageCoordinate : public Resource {
public:
  // Constructors
  ImageCoordinate(uint32_t x, uint32_t y);

public:
  // Getters
  uint32_t get_x() const;
  uint32_t get_y() const;

private:
  uint32_t m_x;
  uint32_t m_y;
};