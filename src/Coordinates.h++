#pragma once

#include "Coordinate.h++"
#include "Costs.h++"

#include <vector>


class Coordinates {

public:

  static std::vector<unsigned int> calculateDimensionalMaxima(const std::vector< std::vector<unsigned int> >& lengths);

};
