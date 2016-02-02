#pragma once

class Distance {

private:

  static const int BIG_DISTANCE = 2500;

  double pnorm(double z);

  int match(int len1, int len2);

public:

  int two_side_distance(int x1, int y1, int x2, int y2);

};
