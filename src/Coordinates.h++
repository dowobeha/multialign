#pragma once

#include "Coordinate.h++"
#include "CoordinateIterator.h++"
#include "Costs.h++"

#include <vector>


class Coordinates {

private:

  const std::vector<unsigned int> dimensional_maximum;

  static std::vector<unsigned int> calculateDimensionalMaxima(const std::vector< std::vector<unsigned int> >& lengths);


public:

  Coordinates(Costs& costs);

  //  Coordinates(std::initializer_list<unsigned int> maxima);
  Coordinates(std::vector<unsigned int> maxima);

  CoordinateIterator begin();

  CoordinateIterator end();

};
