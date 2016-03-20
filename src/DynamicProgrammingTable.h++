#pragma once

#include <map>
#include <ostream>
#include <string>
#include <vector>

#include "Cost.h++"

class DynamicProgrammingTable {

private:

  std::map<Coordinate, Cost> costs;

  std::vector<std::string> languages;

public:

  DynamicProgrammingTable(std::vector<std::string> languages) : languages(languages) {}



  double get(Coordinate c) const;

  void set(Coordinate current, Coordinate previous, double cost);

  unsigned int dimensions() const;

  std::map< std::string, std::vector< int > > backtrace() const;

  double cost() const;

  friend std::ostream& operator<<(std::ostream& os, const DynamicProgrammingTable& t);
  
};

