#include "Coordinates.h++"
#include "Costs.h++"

#include <vector>

Coordinates::Coordinates(Costs& costs) : dimensional_maximum(calculateDimensionalMaxima(costs.lengths)) {
  // This space intentionally left blank
}

CoordinateIterator Coordinates::begin() const {
  return CoordinateIterator(*this, std::vector<unsigned int>(dimensional_maximum.size(), 0));
}

CoordinateIterator Coordinates::end() const {
  return CoordinateIterator(*this, std::vector<unsigned int>(dimensional_maximum.size(), UINT_MAX));
}

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
