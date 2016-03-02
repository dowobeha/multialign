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

  bool contains(std::string language) {
    return values.find(language) == values.end();
  }

  unsigned int numSegments() const {
    unsigned int count = 0;
  
    for (auto value : values.begin()->second) {
      if (value < 0) {
	count += 1;
      }
    }

    return count;
  }

};
