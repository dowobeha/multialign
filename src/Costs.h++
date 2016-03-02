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

  Costs(std::vector<std::string> languages) : languages(languages) {}



  double get(Coordinate c) const;

  void set(Coordinate current, Coordinate previous, double cost);

  unsigned int dimensions() const;

  std::map< std::string, std::vector< int > > backtrace() const;

  double cost() const;

  double twoDimensionalMatchCost(Alignment::Type alignment,
				 unsigned int i, const std::vector<unsigned int>& x,
				 unsigned int j, const std::vector<unsigned int>& y) const;

};

