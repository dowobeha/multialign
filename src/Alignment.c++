#include "Alignment.h++"

Alignment::Type Alignment::determine(unsigned int current_x, unsigned int current_y,
				     unsigned int previous_x, unsigned int previous_y) {

  if (previous_x + 1 == current_x &&
      previous_y + 1 == current_y) {

    return Alignment::Type::Substitution;

  } else if (previous_x + 1 == current_x &&
	     previous_y + 0 == current_y) {

    return Alignment::Type::Deletion;

  } else if (previous_x + 0 == current_x &&
	     previous_y + 1 == current_y) {

    return Alignment::Type::Insertion;

  } else if (previous_x + 2 == current_x &&
	     previous_y + 1 == current_y) {

    return Alignment::Type::Contraction;

  } else if (previous_x + 1 == current_x &&
	     previous_y + 2 == current_y) {

    return Alignment::Type::Expansion;

  } else if (previous_x + 2 == current_x &&
	     previous_y + 2 == current_y) {

    return Alignment::Type::Melding;

  } else if (previous_x == current_x && 
	     previous_y == current_y) {
    
    return Alignment::Type::Equal;

  } else {

    return Alignment::Type::Invalid;

  }

}
