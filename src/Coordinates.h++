#pragma once

#include "Coordinate.h++"
#include "CoordinateIterator.h++"

#include <vector>


class Coordinates {

  const std::vector<unsigned int> dimensional_maximum;


public:

  Coordinates(std::initializer_list<unsigned int> maxima);
  Coordinates(std::vector<unsigned int> maxima);

  CoordinateIterator begin();

  CoordinateIterator end();

};
