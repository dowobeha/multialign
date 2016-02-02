#include "Coordinate.h++"
#include "Coordinates.h++"

#include <algorithm>
#include <ostream>
#include <vector>

Coordinate::Coordinate(const std::vector<unsigned int> maxima, 
		       const std::vector<unsigned int> v) : dimensional_maximum(maxima), 
							    value(v) {
  // This space intentionally left blank
}


unsigned int Coordinate::dimensions() {
  return value.size();
}

unsigned int Coordinate::valueAt(unsigned int dimension) {
  return value[dimension];
}


bool Coordinate::hasPredecessor(Coordinate& c) {

    for (unsigned int dimension=0, num_dimensions=value.size(); dimension<num_dimensions; dimension+=1) {
      if (c.value[dimension] > value[dimension]) {
	return false;
      } else if (c.value[dimension] + 2 < value[dimension]) {
	return false;
      }
    }

    if (*this == c) {
      return false;
    } else {
      return true;
    }

  } 

std::vector<Coordinate> Coordinate::possiblePredecessors() {

    std::vector<Coordinate> results;

    for (Coordinate c : Coordinates{dimensional_maximum}) {
      if (this->hasPredecessor(c)) {
	results.push_back(c);
      }
    }

    return results;
  }

bool Coordinate::operator ==(const Coordinate& that) const {
  return std::equal(value.begin(), value.end(), 
		    that.value.begin(), that.value.end());
}

bool Coordinate::operator <(const Coordinate& that) const {
  return std::lexicographical_compare(value.begin(), value.end(), 
				      that.value.begin(), that.value.end());
}


std::ostream& operator<<(std::ostream& os, const Coordinate& c) {
  
  unsigned int size = c.value.size();
  unsigned int lastIndex = size - 1;
  
  os << '(';
  for (unsigned int index=0; index<lastIndex; index+=1) {
    os << c.value[index] << ',';
  }
  os << c.value[lastIndex] << ')';
  
  return os;
}


