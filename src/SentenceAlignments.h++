#pragma once

#include <vector>

class SentenceAlignments {

public:

  const std::vector< std::vector< int > > value;
  const double cost;

  SentenceAlignments(std::vector< std::vector< int > > value, double cost) : 
    value{value}, cost{cost} {}

};
