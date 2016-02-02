#pragma once

#include <map>
#include <vector>

#include "Cost.h++"
#include "Distance.h++"

class Costs {

private:

  std::map<Coordinate, Cost> costs;

  std::vector< std::vector<unsigned int> > lengths;

  Distance distance;

public:

  Costs(std::vector< std::vector<unsigned int> > lengths) : lengths(lengths) {}

  void calculate(Coordinate current, Coordinate previous);

  Cost get(Coordinate c);

};

