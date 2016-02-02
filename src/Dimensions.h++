#pragma once

#include "DimensionsIterator.h++"

class Dimensions {

private:
  
  const unsigned int num_dimensions;

public:

  Dimensions(const unsigned int num_dimensions);

  DimensionsIterator begin() const;

  DimensionsIterator end() const;

};
