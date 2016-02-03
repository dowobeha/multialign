#pragma once

class Alignment {

public:

  Alignment() = delete;

  enum class Type {
    Substitution, Deletion, Insertion, Contraction, Expansion, Melding, Invalid
  };

  static Type determine(unsigned int x1, unsigned int y1,
			unsigned int x2, unsigned int y2);

};
