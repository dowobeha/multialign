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
