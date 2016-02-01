#pragma once

#include <vector>
#include <algorithm>
#include <climits>

class Coordinate {

private:
  
  std::vector<unsigned int> dimensional_maximum;

  std::vector<unsigned int> value;

public:
  
  Coordinate() = default;
  Coordinate(const Coordinate& c) = default;
  Coordinate(Coordinate&& c) = default;
  Coordinate& operator=(const Coordinate& c) = default;
  Coordinate& operator=(Coordinate&& c) = default;
  ~Coordinate() = default;
  
  Coordinate(const std::vector<unsigned int> maxima, const std::vector<unsigned int> v) : dimensional_maximum(maxima), value(v) {}

  bool hasPredecessor(Coordinate& c);

  std::vector<Coordinate> possiblePredecessors();

  bool operator ==(const Coordinate& that) const;

  bool operator <(const Coordinate& that) const;

  unsigned int dimensions() {
    return value.size();
  }

  unsigned int valueAt(unsigned int dimension) {
    return value[dimension];
  }

  friend std::ostream& operator<<(std::ostream& os, const Coordinate& c);

};

class CoordinateIterator : public std::iterator<std::input_iterator_tag, Coordinate>
{

private:

  const std::vector<unsigned int> dimensional_maximum;

  void nextValue() {

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

  std::vector<unsigned int> value;

public:
  CoordinateIterator(std::vector<unsigned int> maxima, std::vector<unsigned int> v) : dimensional_maximum(maxima), value(v) {}
  CoordinateIterator(const CoordinateIterator& mit) : dimensional_maximum(mit.dimensional_maximum), value(mit.value) {}
  CoordinateIterator& operator++() {nextValue();return *this;}

  bool operator==(const CoordinateIterator& rhs) {
    return std::equal(value.begin(), value.end(), 
                      rhs.value.begin(), rhs.value.end());

  }
  bool operator!=(const CoordinateIterator& rhs) {return !(*this==rhs);}
  Coordinate operator*() {return Coordinate(dimensional_maximum, value);}
};


class Coordinates {

  const std::vector<unsigned int> dimensional_maximum;


public:

  Coordinates(std::initializer_list<unsigned int> maxima) : dimensional_maximum(maxima) {}
  Coordinates(std::vector<unsigned int> maxima) : dimensional_maximum(maxima) {}

  CoordinateIterator begin() {
    return CoordinateIterator(dimensional_maximum, std::vector<unsigned int>(dimensional_maximum.size(), 0));
  }

  CoordinateIterator end() {
    return CoordinateIterator(dimensional_maximum, std::vector<unsigned int>(dimensional_maximum.size(), UINT_MAX));
  }

};


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
