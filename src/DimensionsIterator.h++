#pragma once

#include <iterator>
#include <utility>

class DimensionsIterator : public std::iterator<std::input_iterator_tag, std::pair<unsigned int, unsigned int> > {

private:
  
  unsigned int x;
  unsigned int y;
  const unsigned int max;
  
  DimensionsIterator(const unsigned int max, const unsigned int x, const unsigned int y);
  
public:

  DimensionsIterator& operator++();
  bool operator==(const DimensionsIterator& rhs) const;
  bool operator!=(const DimensionsIterator& rhs) const;
  std::pair<unsigned int, unsigned int> operator*() const;

  static DimensionsIterator begin(const unsigned int max);
  static DimensionsIterator end(const unsigned int max);

};
