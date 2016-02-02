#include "Dimensions.h++"
#include "DimensionsIterator.h++"

Dimensions::Dimensions(const unsigned int num_dimensions) : num_dimensions(num_dimensions) {
  // This space intentionally left blank
}

DimensionsIterator Dimensions::begin() const {
  return DimensionsIterator::begin(num_dimensions);
}

DimensionsIterator Dimensions::end() const {
  return DimensionsIterator::end(num_dimensions);
}

