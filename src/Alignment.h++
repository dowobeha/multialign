#pragma once

#include <ostream>

class Alignment {

public:

  Alignment() = delete;

  enum class Type {
    Substitution, Deletion, Insertion, Contraction, Expansion, Melding, Equal, Invalid
  };

  static Type determine(unsigned int x1, unsigned int y1,
			unsigned int x2, unsigned int y2);

  friend std::ostream& operator<<(std::ostream& os, const Type& a);

};
