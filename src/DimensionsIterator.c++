#include "DimensionsIterator.h++"

#include <climits>
#include <utility>
#include <iostream>

DimensionsIterator::DimensionsIterator(const unsigned int max, const unsigned int x, const unsigned int y) : max(max), x(x), y(y) {
  // This space intentionally left blank
}

bool DimensionsIterator::operator==(const DimensionsIterator& rhs) const {
  if (x==rhs.x && y==rhs.y && max==rhs.max) {
    return true;
  } else {
    return false;
  }
}

bool DimensionsIterator::operator!=(const DimensionsIterator& rhs) const {
  return !(*this==rhs);
}

DimensionsIterator& DimensionsIterator::operator++() {
  
  //  std::cerr << "Was x==" << x << " y==" << y << " max==" << max << std::endl;
  
  if (y!=UINT_MAX && y+1 < max) {
    y += 1;
  } else if (x!=UINT_MAX && x+1!=UINT_MAX && x+2 < max) {
    x += 1;
    y = x+1;
  } else {
    y = UINT_MAX;
    x = UINT_MAX;
  }

  //  std::cerr << "Now x==" << x << " y==" << y << " max==" << max << std::endl;

  return *this;

}

std::pair<unsigned int, unsigned int> DimensionsIterator::operator*() const {
  return std::pair<unsigned int, unsigned int>(x, y);
}

DimensionsIterator DimensionsIterator::begin(const unsigned int max) {

  //  std::cerr << "Constructing begin" << std::endl;

  if (max > 1) {
    return DimensionsIterator(max, 0, 1);
  } else {
    return DimensionsIterator::end(max);
  }
}


DimensionsIterator DimensionsIterator::end(const unsigned int max) {

  return DimensionsIterator(max, UINT_MAX, UINT_MAX);
  
}
