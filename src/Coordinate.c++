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

  // If a coordinate is identical in all dimensions to this coordinate,
  //    then it couldn't be a predecessor of this coordinate
  if (*this == c) return false;

  // Iterate over every pair of dimensions
  for (unsigned int dimension1=0, num_dimensions=value.size(); dimension1<num_dimensions; dimension1 += 1) {

    for (unsigned int dimension2=dimension1+1; dimension2<num_dimensions; dimension2 += 1) {

      if (value[dimension1]==c.value[dimension1] &&
	  value[dimension2]==c.value[dimension2]) {

	// If a coordinate is identical in all dimensions to this coordinate,
	//    then it couldn't be a predecessor of this coordinate.
	//
	// However, it's OK for any two dimensions to have identical values,
	//    as long as some other pair of dimensions have differing values.
	//
	continue;

      } else if (value[dimension1]==c.value[dimension1]+1 &&
		 value[dimension2]==c.value[dimension2]+1) {

	// It's OK for a predecessor coordinate 
	//    to differ in value by -1 in the one dimension and also
	//    to differ in value by -1 in the other dimension.
	//
	// This corresponds with a 1-1 sentence alignment (substitution)
	//
	continue;

      } else if (value[dimension1]==c.value[dimension1]+1 &&
		 value[dimension2]==c.value[dimension2]) {

	// It's OK for a predecessor coordinate 
	//    to differ in value by -1 in one dimension and
	//    to be identical in the other dimension
	//
	// This corresponds with a 0-1 sentence alignment (insertion)
	//
	continue;

      } else if (value[dimension1]==c.value[dimension1] &&
		 value[dimension2]==c.value[dimension2]+1) {

	// It's OK for a predecessor coordinate 
	//    to be identical in one dimension and
	//    to differ in value by -1 in the other dimension.
	//
	// This corresponds with a 1-0 sentence alignment (deletion)
	//
	continue;

      } else if (value[dimension1]==c.value[dimension1]+1 &&
		 value[dimension2]==c.value[dimension2]+2) {

	// It's OK for a predecessor coordinate 
	//    to differ in value by -1 in one dimension and
	//    to differ in value by -2 in the other dimension.
	//
	// This corresponds with a 1-2 sentence alignment (expansion)
	//
	continue;

      } else if (value[dimension1]==c.value[dimension1]+2 &&
		 value[dimension2]==c.value[dimension2]+1) {

	// It's OK for a predecessor coordinate 
	//    to differ in value by -1 in one dimension and
	//    to differ in value by -2 in the other dimension.
	//
	// This corresponds with a 2-1 sentence alignment (contraction)
	//
	continue;

      } else if (value[dimension1]==c.value[dimension1]+2 &&
		 value[dimension2]==c.value[dimension2]+2) {

	// It's OK for a predecessor coordinate 
	//    to differ in value by -2 in one dimension and
	//    to differ in value by -2 in the other dimension.
	//
	// This corresponds with a 2-2 sentence alignment (melding)
	//
	continue;

      } else {

	// All other types of alignments are not allowed

	return false;

      }

    }

  }

  return true;

  /*
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
  */
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


