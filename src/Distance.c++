#include "Distance.h++"

#include <cmath>
#include <cstdio>
#include <string>

#include "Alignment.h++"

/* Returns the area under a normal distribution
   from -inf to z standard deviations */
double Distance::pnorm(double z)
{
  double t, pd;
  t = 1/(1 + 0.2316419 * z);
  pd = 1 - 0.3989423 *
    exp(-z * z/2) *
    ((((1.330274429 * t - 1.821255978) * t
       + 1.781477937) * t - 0.356563782) * t + 0.319381530) * t;
  /* see Abramowitz, M., and I. Stegun (1964), 26.2.17 p. 932 */
  return(pd);
}

/* Return -100 * log probability that an English sentence of length
   len1 is a translation of a foreign sentence of length len2. The
   probability is based on two parameters, the mean and variance of
   number of foreign characters per English character.
*/
int Distance::match(int len1, int len2)
{
  double z, pd, mean;
  double c = 1;
  double s2 = 6.8 ;

  if(len1==0 && len2==0) return(0);
  mean = (len1 + len2/c)/2;
  z = (c * len1 - len2)/sqrt(s2 * mean);

  /* Need to deal with both sides of the normal distribution */
  if(z < 0) z = -z;
  pd = 2 * (1 - pnorm(z));

  if(pd > 0) return((int)(-100 * log(pd)));
  else return(BIG_DISTANCE);
}

int Distance::penalty(Alignment::Type type) {

  switch(type) {
      
  case Alignment::Type::Substitution: 
    return penalty11;
      
  case Alignment::Type::Deletion:
    return penalty01;

  case Alignment::Type::Insertion:
    return penalty01;
      
  case Alignment::Type::Contraction:
    return penalty21;
      
  case Alignment::Type::Expansion:
    return penalty21;
  
  case Alignment::Type::Melding:
    return penalty22;
    
  case Alignment::Type::Invalid:
    return BIG_DISTANCE;
      
  }

}
/*
int two_side_distance(int x1, int y1, int x2, int y2)
{

  int match_value;
  int penalty_value;
  int return_value;

  std::string operation;

  if(x2 == 0 && y2 == 0)

    if(x1 == 0) {                 // insertion : 0-1 alignment 
      match_value   = match(x1, y1);
      penalty_value = penalty01;
      operation = "insertion";
      //      return(match(x1, y1) + penalty01);
    }

    else if(y1 == 0) {            // deletion : 1-0 alignment 
      match_value = match(x1, y1);
      penalty_value = penalty01;
      operation = "deletion";
      //  return(match(x1, y1) + penalty01);
    }

    else {
      match_value = match(x1, y1);
      penalty_value = 0;
      operation = "substitution";
      // return (match(x1, y1)); // substitution : 1-1 alignment 
    }

  else if(x2 == 0) {              // expansion : 1-2 alignment 
    match_value = match(x1, y1 + y2);
    penalty_value = penalty21;
    operation = "expansion";
    //    return (match(x1, y1 + y2) + penalty21);
  }

  else if(y2 == 0) {              // contraction : 2-1 alignment 
    match_value = match(x1 + x2, y1);
    penalty_value = penalty21;
    operation = "contraction";
    //    return(match(x1 + x2, y1) + penalty21);
  }

  else {                          // merger : 2-2 alignment 
    match_value = match(x1 + x2, y1 + y2);
    penalty_value = penalty22;
    operation = "merger";
    //    return(match(x1 + x2, y1 + y2) + penalty22);
  }

  return_value  = match_value + penalty_value;

  fprintf(stderr, "%s\tdistance(x1=%d, y1=%d, x2=%d, y2=%d) = %d = match_value (%d) + penalty_value (%d)\n", operation.c_str(), x1, y1, x2, y2, return_value, match_value, penalty_value);

  return return_value;

}
*/
