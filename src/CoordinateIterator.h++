#pragma once

#include "Coordinate.h++"

#include <iterator>
#include <vector>

class Coordinates;

class CoordinateIterator : public std::iterator<std::input_iterator_tag, Coordinate> {

private:
  
  const Coordinates& coordinates;

  const std::vector<unsigned int> dimensional_maximum;

  std::vector<unsigned int> value;

  void nextValue();

public:

  CoordinateIterator(const Coordinates& c, std::vector<unsigned int> v);

  CoordinateIterator& operator++();

  bool operator==(const CoordinateIterator& rhs) const;

  bool operator!=(const CoordinateIterator& rhs) const;

  Coordinate operator*() const;

};
