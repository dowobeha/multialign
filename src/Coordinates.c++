#include "Coordinates.h++"

Coordinates::Coordinates(std::initializer_list<unsigned int> maxima) : dimensional_maximum(maxima) {
  // This space intentionally left blank
}

Coordinates::Coordinates(std::vector<unsigned int> maxima) : dimensional_maximum(maxima) {
  // This space intentionally left blank 
}


CoordinateIterator Coordinates::begin() {
  return CoordinateIterator(dimensional_maximum, std::vector<unsigned int>(dimensional_maximum.size(), 0));
}

CoordinateIterator Coordinates::end() {
  return CoordinateIterator(dimensional_maximum, std::vector<unsigned int>(dimensional_maximum.size(), UINT_MAX));
}
