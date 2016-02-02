#pragma once

#include <vector>

// To prevent a circular header dependency, forward declare Coordinates
class Coordinates;


class Coordinate {

private:
  
  const Coordinates& coordinates;

  const std::vector<unsigned int> value;

public:

  Coordinate(const Coordinates& coordinates, const std::vector<unsigned int> v);

  bool hasPredecessor(Coordinate& c) const;

  std::vector<Coordinate> possiblePredecessors() const;

  bool operator ==(const Coordinate& that) const;

  bool operator <(const Coordinate& that) const;

  unsigned int dimensions() const;

  unsigned int valueAt(unsigned int dimension) const;

  friend std::ostream& operator<<(std::ostream& os, const Coordinate& c);

};

