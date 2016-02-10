#include "Coordinate.h++"
#include "Coordinates.h++"
#include "Dimensions.h++"

#include <algorithm>
#include <iostream>
#include <ostream>
#include <vector>

Coordinate::Coordinate(const std::vector<unsigned int> max) : value(max.size(), 0), max(max) {
  // This space intentionally left blank
}


unsigned int Coordinate::dimensions() const {
  return value.size();
}

unsigned int Coordinate::valueAt(unsigned int dimension) const {
  return value[dimension];
}


bool Coordinate::hasPredecessor(Coordinate& c) const {

  // If a coordinate is identical in all dimensions to this coordinate,
  //    then it couldn't be a predecessor of this coordinate
  if (*this == c) return false;

  // Iterate over every pair of dimensions
  for (auto dimensions : Dimensions(value.size())) {

    Alignment::Type alignment = 
      Alignment::determine(this->valueAt(dimensions.first),  this->valueAt(dimensions.second),
			   c.valueAt(dimensions.first), c.valueAt(dimensions.second));

    if (alignment == Alignment::Type::Invalid) return false;

  }

  return true;

} 

bool Coordinate::operator ==(const Coordinate& that) const {
  return std::equal(value.begin(), value.end(), 
		    that.value.begin(), that.value.end());
}

bool Coordinate::operator <(const Coordinate& that) const {
  return std::lexicographical_compare(value.begin(), value.end(), 
				      that.value.begin(), that.value.end());
}

void Coordinate::increment() {



  
  for (unsigned int dimension=0, num_dimensions=value.size(); dimension < num_dimensions; dimension += 1) {
    if (value[dimension] + 1 <= max[dimension]) {
      value[dimension] += 1;

      break;
    } else {
      value[dimension] = 0;
    }
  }
  


}

bool Coordinate::canIncrement() {

  for (unsigned int dimension=0, num_dimensions=value.size(); dimension < num_dimensions; dimension += 1) {
    
    if (value[dimension] < max[dimension]) {
      return true;
    }
    
  }

  return false;

}

bool Coordinate::resetToEarliestPredecessorOf(Coordinate &c) {

  for (unsigned int dimension=0, num_dimensions=value.size(); dimension < num_dimensions; dimension += 1) {

    max[dimension] = c.value[dimension];

    if (value[dimension] >= 2) {
      value[dimension] -= 2;
    } else {
      value[dimension] = 0;
    }
    
  }

  return (*this) < c;

}

std::ostream& operator<<(std::ostream& os, const Coordinate& c) {
  
  unsigned int size = c.value.size();
  
  os << '(';
  for (unsigned int index=size-1; index>0; index-=1) {
    os << c.value[index] << ',';
  }
  os << c.value[0] << ')';
  
  return os;
}


