#pragma once

#include "Alignment.h++"

#include <vector>

class Gale_and_Church_1993 {

private:

  static const int BIG_DISTANCE = 2500;

  static const int penalty21 = 230;  /* -100 * log([prob of 2-1 match] / [prob of 1-1 match]) */
  static const int penalty22 = 440;  /* -100 * log([prob of 2-2 match] / [prob of 1-1 match]) */
  static const int penalty01 = 450;  /* -100 * log([prob of 0-1 match] / [prob of 1-1 match]) */
  static const int penalty11 = 0;    /* -100 * log([prob of 1-1 match] / [prob of 1-1 match]) */

  static double pnorm(double z);

public:

  static int penalty(Alignment::Type type);
  static int match(int len1, int len2);
  static int maxCost() { return BIG_DISTANCE; }

  static double twoDimensionalMatchCost(Alignment::Type alignment,
					unsigned int i, const std::vector<unsigned int>& x,
					unsigned int j, const std::vector<unsigned int>& y);

};
