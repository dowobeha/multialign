#pragma once

#include "Coordinate.h++"

class Cost {

private:

  Coordinate previous;

public:

  const double cost;
  /*
  Cost() = default;
  Cost(const Cost& c) = default;
  Cost(Cost&& c) = default;
  Cost& operator=(const Cost& c) = default;
  Cost& operator=(Cost&& c) = default;
  ~Cost() = default;
  */
  Cost(Coordinate previous, double cost) : previous{previous}, cost{cost} {}


};
