#include "Coordinate.h++"
#include "CoordinateIterator.h++"

#include <algorithm>
#include <climits>
#include <vector>

void CoordinateIterator::nextValue() {

  bool changed = false;
  
  for (unsigned int dimension=0, num_dimensions=value.size(); dimension < num_dimensions; dimension += 1) {
    if (value[dimension] + 1 <= dimensional_maximum[dimension]) {
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

CoordinateIterator::CoordinateIterator(std::vector<unsigned int> maxima, 
				       std::vector<unsigned int> v) : dimensional_maximum(maxima), 
								      value(v) {
  // This space intentionally left blank
}

CoordinateIterator::CoordinateIterator(const CoordinateIterator& mit) : dimensional_maximum(mit.dimensional_maximum), 
									value(mit.value) {
  // This space intentionally left blank
}

CoordinateIterator& CoordinateIterator::operator++() {
  nextValue();
  return *this;
}

bool CoordinateIterator::operator==(const CoordinateIterator& rhs) {
  return std::equal(value.begin(), value.end(), 
		    rhs.value.begin(), rhs.value.end());
  
}

bool CoordinateIterator::operator!=(const CoordinateIterator& rhs) {
  return !(*this==rhs);
}

Coordinate CoordinateIterator::operator*() {
  return Coordinate(dimensional_maximum, value);
}
