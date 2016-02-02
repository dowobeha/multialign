#pragma once

#include <vector>

class Coordinate {

private:
  
  std::vector<unsigned int> dimensional_maximum;

  std::vector<unsigned int> value;

public:
  /*  
  Coordinate() = default;
  Coordinate(const Coordinate& c) = default;
  Coordinate(Coordinate&& c) = default;
  Coordinate& operator=(const Coordinate& c) = default;
  Coordinate& operator=(Coordinate&& c) = default;
  ~Coordinate() = default;
  */
  Coordinate(const std::vector<unsigned int> maxima, const std::vector<unsigned int> v);

  bool hasPredecessor(Coordinate& c);

  std::vector<Coordinate> possiblePredecessors();

  bool operator ==(const Coordinate& that) const;

  bool operator <(const Coordinate& that) const;

  unsigned int dimensions();

  unsigned int valueAt(unsigned int dimension);

  friend std::ostream& operator<<(std::ostream& os, const Coordinate& c);

};

