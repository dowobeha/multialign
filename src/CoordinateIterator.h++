#pragma once

#include "Coordinate.h++"
#include "CoordinateIterator.h++"

#include <vector>

class CoordinateIterator : public std::iterator<std::input_iterator_tag, Coordinate> {

private:

  const std::vector<unsigned int> dimensional_maximum;

  std::vector<unsigned int> value;

  void nextValue();

public:

  CoordinateIterator(std::vector<unsigned int> maxima, std::vector<unsigned int> v);
  CoordinateIterator(const CoordinateIterator& mit);

  CoordinateIterator& operator++();

  bool operator==(const CoordinateIterator& rhs);

  bool operator!=(const CoordinateIterator& rhs);

  Coordinate operator*();

};
