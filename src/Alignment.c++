#include "Alignment.h++"

#include <ostream>

Alignment::Type Alignment::convert(unsigned int n1, unsigned int n2) {

  if (n1==0 && n2==0) {
    return Alignment::Type::Equal;
  } else if (n1==0 && n2==1) {
    return Alignment::Type::Insertion;
  } else if (n1==1 && n2==0) {
    return Alignment::Type::Deletion;
  } else if (n1==1 && n2==1) {
    return Alignment::Type::Substitution;
  } else if (n1==1 && n2==2) {
    return Alignment::Type::Expansion;
  } else if (n1==2 && n2==1) {
    return Alignment::Type::Contraction;
  } else if (n1==2 && n2==2) {
    return Alignment::Type::Melding;
  } else {
    return Alignment::Type::Invalid;
  }

}

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

std::ostream& operator<<(std::ostream& os, const Alignment::Type& alignment) {

  switch(alignment) {
      
  case Alignment::Type::Substitution: 
    os << "1-1 (Substitution)";
    break;
      
  case Alignment::Type::Deletion:
    os << "1-0 (Deletion)";
    break;
      
  case Alignment::Type::Insertion:
    os << "0-1 (Insertion)";
    break;
      
  case Alignment::Type::Contraction:
    os << "2-1 (Contraction)";
    break;
      
  case Alignment::Type::Expansion:
    os << "1-2 (Expansion)";
    break;
            
  case Alignment::Type::Melding:
    os << "2-2 (Melding)";
    break;

  case Alignment::Type::Equal:
    os << "0-0 (Equal)";
    break;
  
  case Alignment::Type::Invalid:
  default:
    os << "?-? (Invalid)";
    break;
      
  }

  return os;

}
