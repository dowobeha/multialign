#pragma once

#include <vector>
#include <ostream>

// To prevent a circular header dependency, forward declare Coordinates
class Coordinates;


class Coordinate {

private:
  
  std::vector<unsigned int> max;

  std::vector<unsigned int> value;

public:

  Coordinate(const std::vector<unsigned int> max);

  Coordinate() = delete;
  Coordinate& operator=(const Coordinate&) = delete;

  bool hasPredecessor(Coordinate& c) const;

  bool operator ==(const Coordinate& that) const;

  bool operator <(const Coordinate& that) const;

  unsigned int dimensions() const;

  unsigned int valueAt(unsigned int dimension) const;

  bool canIncrement();

  void increment();

  bool resetToEarliestPredecessorOf(Coordinate &c);

  friend std::ostream& operator<<(std::ostream& os, const Coordinate& c);

};

