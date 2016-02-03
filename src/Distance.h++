#pragma once

#include "Alignment.h++"

class Distance {

private:

  static const int BIG_DISTANCE = 2500;

  static const int penalty21 = 230;  /* -100 * log([prob of 2-1 match] / [prob of 1-1 match]) */
  static const int penalty22 = 440;  /* -100 * log([prob of 2-2 match] / [prob of 1-1 match]) */
  static const int penalty01 = 450;  /* -100 * log([prob of 0-1 match] / [prob of 1-1 match]) */
  static const int penalty11 = 0;    /* -100 * log([prob of 1-1 match] / [prob of 1-1 match]) */

  double pnorm(double z);

public:

  int penalty(Alignment::Type type);
  int match(int len1, int len2);

  //  int two_side_distance(Alignment::Type type);

};