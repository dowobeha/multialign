#pragma once

#include <map>
#include <string>
#include <vector>

#include "Cost.h++"
#include "Distance.h++"

class Costs {

private:

  std::map<Coordinate, Cost> costs;

  Distance distance;

  std::vector<std::string> languages;

public:

  const std::vector< std::vector<unsigned int> > lengths;

  Costs(std::vector< std::vector<unsigned int> > lengths, std::vector<std::string> languages) : lengths(lengths), languages(languages) {}

  void calculate(Coordinate current, Coordinate previous);

  Cost get(Coordinate c) const;

  unsigned int dimensions() const;

  std::map< std::string, std::vector< int > > backtrace() const;

  double cost() const;

};

