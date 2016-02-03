#pragma once

#include <map>
#include <vector>

#include "Cost.h++"
#include "Distance.h++"

class Costs {

private:

  std::map<Coordinate, Cost> costs;

  Distance distance;

public:

  const std::vector< std::vector<unsigned int> > lengths;

  Costs(std::vector< std::vector<unsigned int> > lengths) : lengths(lengths) {}

  void calculate(Coordinate current, Coordinate previous);

  Cost get(Coordinate c) const;

  unsigned int dimensions() const;

};

