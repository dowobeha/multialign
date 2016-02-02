#pragma once

#include "Coordinate.h++"
#include "CoordinateIterator.h++"
#include "Costs.h++"

#include <vector>


class Coordinates {

private:

  static std::vector<unsigned int> calculateDimensionalMaxima(const std::vector< std::vector<unsigned int> >& lengths);


public:

  const std::vector<unsigned int> dimensional_maximum;

  Coordinates(Costs& costs);

  CoordinateIterator begin() const;

  CoordinateIterator end() const;

};
