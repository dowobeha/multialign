#include "Coordinates.h++"

#include <vector>

std::vector<unsigned int> Coordinates::calculateDimensionalMaxima(const std::vector< std::vector<unsigned int> >& lengths) {

  std::vector<unsigned int> result;

  for (const std::vector<unsigned int>& l : lengths) {
    // In order to make the length vectors use 1-based indexing,
    //    the first element of each length vector is zero,
    //    which we will ignore by subtracting 1 from l.size()
    result.push_back(l.size() - 1);
  } 

  return result;

}
