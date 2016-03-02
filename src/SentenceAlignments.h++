#pragma once

#include <limits>
#include <string>
#include <vector>

#include "Costs.h++"

class SentenceAlignments {

public:

  std::map< std::string, std::vector< int > > values;
  double cost;

  SentenceAlignments() : values{}, cost{std::numeric_limits<double>::max()} {}

  SentenceAlignments(Costs costs) : 
    values{costs.backtrace()}, cost{costs.cost()} {}

};
