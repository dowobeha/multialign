#include "Coordinate.h++"
#include "CoordinateIterator.h++"
#include "Coordinates.h++"

#include <algorithm>
#include <climits>
#include <vector>

void CoordinateIterator::nextValue() {

  bool changed = false;
  
  for (unsigned int dimension=0, num_dimensions=value.size(); dimension < num_dimensions; dimension += 1) {
    if (value[dimension] + 1 <= coordinates.dimensional_maximum[dimension]) {
      value[dimension] += 1;
      changed = true;
      break;
    } else {
      value[dimension] = 0;
    }
  }
  
  if (! changed) {
    for (unsigned int dimension=0, num_dimensions=value.size(); dimension < num_dimensions; dimension += 1) {
      value[dimension] = UINT_MAX;
    }
  }
}

CoordinateIterator::CoordinateIterator(const Coordinates& coordinates,
				       std::vector<unsigned int> v) : coordinates(coordinates),
								      value(v) {
  // This space intentionally left blank
}

CoordinateIterator& CoordinateIterator::operator++() {
  nextValue();
  return *this;
}

bool CoordinateIterator::operator==(const CoordinateIterator& rhs) const {
  return std::equal(value.begin(), value.end(), 
		    rhs.value.begin(), rhs.value.end());
  
}

bool CoordinateIterator::operator!=(const CoordinateIterator& rhs) const {
  return !(*this==rhs);
}

Coordinate CoordinateIterator::operator*() const {
  return Coordinate(coordinates, value);
}
