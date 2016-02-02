#include "Costs.h++"

#include <iostream>
#include <map>
#include <vector>

#include "Cost.h++"
#include "Distance.h++"

Cost Costs::get(Coordinate c) const {
    
  std::map<Coordinate, Cost>::const_iterator search = costs.find(c);
  std::pair<Coordinate, Cost> result = *search;
  
  return result.second;
 
}


void Costs::calculate(Coordinate current, Coordinate previous) {

  if (! (previous < current)) {
    return;
  }

  double cost = 0.0;

  for (unsigned int dimension1=0, numDimensions=current.dimensions(); dimension1<numDimensions; dimension1 += 1) {

    auto current_dim1_value = current.valueAt(dimension1);
    auto prev_dim1_value = previous.valueAt(dimension1);


    for (unsigned int dimension2=dimension1+1; dimension2<numDimensions; dimension2 += 1) {

      //              std::cerr << "Dimension 1 = " << dimension1 << ", Dimension 2 = " << dimension2 << std::endl;

      auto current_dim2_value = current.valueAt(dimension2);
      auto prev_dim2_value = previous.valueAt(dimension2);

      auto x = lengths[dimension1];
      auto y = lengths[dimension2];

      auto nx = x.size();
      auto ny = y.size();

      auto i = current_dim1_value;
      auto j = current_dim2_value;

      //      auto x1 = x[i];
      //      auto y1 = y[j];

      //      auto x2 = x[prev_dim1_value];
      //      auto y2 = y[prev_dim2_value];

      //  std::cerr << "(x1=" << x1 << ",y1="<<y1<<") vs " << current << std::endl;
      //std::cerr << "(x2=" << x2 << ",y2="<<y2<<") vs " << previous << std::endl;

      //  std::cerr << "prev_dim1_value=" << prev_dim1_value << " vs " << "current_dim1_value="<<current_dim1_value << std::endl;
      // std::cerr << "prev_dim2_value=" << prev_dim2_value << " vs " << "current_dim2_value="<<current_dim2_value << std::endl;
      //      std::cerr << "x1="<<x1<<"\ty1="<<y1<<std::endl;
      //      std::cerr << "x1="<<x2<<"\ty1="<<y2<<std::endl;


      if (prev_dim1_value + 1 == current_dim1_value &&
	  prev_dim2_value + 1 == current_dim2_value) {

	// 1-1 alignment (substitution)
	//	std::cerr << "substitution" << std::endl;
	cost += distance.two_side_distance(x[i], y[j], 0, 0);

      } else if (prev_dim1_value + 1 == current_dim1_value &&
		 prev_dim2_value + 0 == current_dim2_value) {

	// 1-0 alignment (deletion)
	// std::cerr << "deletion" << std::endl;
	cost += distance.two_side_distance(x[i], 0, 0, 0);

      } else if (prev_dim1_value + 0 == current_dim1_value &&
		 prev_dim2_value + 1 == current_dim2_value) {

	// 0-1 alignment (insertion)
	// std::cerr << "insertion" << std::endl;
	cost += distance.two_side_distance(0, y[j], 0, 0);

      } else if (prev_dim1_value + 2 == current_dim1_value &&
		 prev_dim2_value + 1 == current_dim2_value) {

	// 2-1 alignment (contraction)
	// std::cerr << "contraction" << std::endl;
	cost += distance.two_side_distance(x[i-1], y[j], x[i], 0);

      } else if (prev_dim1_value + 1 == current_dim1_value &&
		 prev_dim2_value + 2 == current_dim2_value) {

	// 1-2 alignment (expansion)
	// std::cerr << "expansion" << std::endl;
	cost += distance.two_side_distance(x[i], y[j-1], 0, y[j]);

      } else if (prev_dim1_value + 2 == current_dim1_value &&
		 prev_dim2_value + 2 == current_dim2_value) {

	// 2-2 alignment (melding)
	// std::cerr << "melding" << std::endl;
	cost += distance.two_side_distance(x[i-1], y[j-1], x[i], y[j]);

      } else {

	return;
	// Other alignment, which we will ignore
	//		std::cerr << "Skipping:\tcurrent=" << current << "\tprevious=" << previous << std::endl;
      }

    }

  } 

  std::map<Coordinate, Cost>::iterator search = costs.find(current);

  if (search == costs.end() || cost < search->second.cost) {

    costs.emplace(current, Cost(previous, cost));

    //    costs[current] = Cost(previous, cost);
    std::cerr << "New best cost " << cost << " from " << previous << " to " << current << std::endl;

  }
  

}
