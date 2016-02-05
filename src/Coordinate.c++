#include "Coordinate.h++"
#include "Coordinates.h++"
#include "Dimensions.h++"

#include <algorithm>
#include <iostream>
#include <ostream>
#include <vector>

Coordinate::Coordinate(const Coordinates& coordinates,
		       const std::vector<unsigned int> v) : coordinates(coordinates),
							    value(v) {
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

    switch(alignment) {
      
    case Alignment::Type::Substitution: 
      break;
      
    case Alignment::Type::Deletion:
      break;
      
    case Alignment::Type::Insertion:
      break;
      
    case Alignment::Type::Contraction:
      break;
      
    case Alignment::Type::Expansion:
      break;
            
    case Alignment::Type::Melding:
      break;

    case Alignment::Type::Equal:
      break;
  
    case Alignment::Type::Invalid:
      return false;
      
    }

  }

  return true;

} 

std::vector<Coordinate> Coordinate::possiblePredecessors() const {

    std::vector<Coordinate> results;

    for (Coordinate c : coordinates) {
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


