#include "Costs.h++"

#include <iostream>
#include <map>
#include <vector>

#include "Alignment.h++"
#include "Cost.h++"
#include "Dimensions.h++"
#include "Distance.h++"

unsigned int Costs::dimensions() const {
  return lengths.size();
}

Cost Costs::get(Coordinate c) const {
    
  std::map<Coordinate, Cost>::const_iterator search = costs.find(c);
  std::pair<Coordinate, Cost> result = *search;
  
  return result.second;
 
}


void Costs::calculate(Coordinate current, Coordinate previous) {

  double cost = 0.0;

  for (std::pair<unsigned int, unsigned int> dimensions : Dimensions(dimensions())) {
    
    auto x = lengths[dimensions.first];
    auto y = lengths[dimensions.second];
    
    auto i = current.valueAt(dimensions.first);
    auto j = current.valueAt(dimensions.second);
        
    Alignment::Type alignment = 
      Alignment::determine(current.valueAt(dimensions.first),  current.valueAt(dimensions.second),
			   previous.valueAt(dimensions.first), previous.valueAt(dimensions.second));

    int penalty_value = distance.penalty(alignment);
    int match_value;
    std::string operation;

    // Define a local lambda function with reference access to all local variables
    auto match = [&](int x1, int y1, int x2, int y2) {
      match_value = distance.match(x1+x2, y1+y2);
      fprintf(stderr, "%s\tdistance(x1=%d, y1=%d, x2=%d, y2=%d) = %d = match_value (%d) + penalty_value (%d)\n", operation.c_str(), x1, y1, x2, y2, (match_value+penalty_value), match_value, penalty_value);     
    };

    
    switch(alignment) {
      
    case Alignment::Type::Substitution: 
      operation = "substitution";
      match(x[i], y[j], 0, 0);
      break;
      
    case Alignment::Type::Deletion:
      operation = "deletion";
      match(x[i], 0, 0, 0);
      break;
      
    case Alignment::Type::Insertion:
      operation = "insertion";
      match(0, y[j], 0, 0);
      break;
      
    case Alignment::Type::Contraction:
      operation = "contraction";
      match(x[i-1], y[j], x[i], 0);
      break;
      
    case Alignment::Type::Expansion:
      operation   = "expansion";
      match(x[i], y[j-1], 0, y[j]);
      break;
            
    case Alignment::Type::Melding:
      operation   = "melding";
      match(x[i-1], y[j-1], x[i], y[j]);
      break;
      
    case Alignment::Type::Invalid:
      return;
      
    }

    cost += penalty_value + match_value;
    
  } 
  
  std::map<Coordinate, Cost>::iterator search = costs.find(current);

  if (search == costs.end() || cost < search->second.cost) {

    // Associate the new cost with the current point
    costs.emplace(current, Cost(previous, cost));

    std::cerr << "New best cost " << cost << " from " << previous << " to " << current << std::endl;

  }
  

}
