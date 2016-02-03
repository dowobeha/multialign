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

  if (! (previous < current)) {
    return;
  }

  double cost = 0.0;

  for (std::pair<unsigned int, unsigned int> dimensions : Dimensions(dimensions())) {
    
    auto current_dim1_value = current.valueAt(dimensions.first);
    auto prev_dim1_value = previous.valueAt(dimensions.first);

    
    auto current_dim2_value = current.valueAt(dimensions.second);
    auto prev_dim2_value = previous.valueAt(dimensions.second);
    
    auto x = lengths[dimensions.first];
    auto y = lengths[dimensions.second];
    
    auto i = current_dim1_value;
    auto j = current_dim2_value;
    
    
    Alignment::Type alignment = Alignment::determine(current_dim1_value, current_dim2_value,
						     prev_dim1_value, prev_dim2_value);

    int penalty_value = distance.penalty(alignment);
    int match_value;
    std::string operation;
    
    switch(alignment) {
      
    case Alignment::Type::Substitution: 
      match_value = distance.match(x[i], y[j]);
      operation = "substitution";
      fprintf(stderr, "%s\tdistance(x1=%d, y1=%d, x2=%d, y2=%d) = %d = match_value (%d) + penalty_value (%d)\n", operation.c_str(), x[i], y[j], 0, 0, (match_value+penalty_value), match_value, penalty_value);
      //      cost += distance.two_side_distance(x[i], y[j], 0, 0);
      break;
      
    case Alignment::Type::Deletion:
      match_value = distance.match(x[i], 0);
      operation = "deletion";
      fprintf(stderr, "%s\tdistance(x1=%d, y1=%d, x2=%d, y2=%d) = %d = match_value (%d) + penalty_value (%d)\n", operation.c_str(), x[i], 0, 0, 0, (match_value+penalty_value), match_value, penalty_value);
      //      cost += distance.two_side_distance(x[i], 0, 0, 0);
      break;
      
    case Alignment::Type::Insertion:
      match_value = distance.match(0, y[j]);
      operation = "insertion";
      fprintf(stderr, "%s\tdistance(x1=%d, y1=%d, x2=%d, y2=%d) = %d = match_value (%d) + penalty_value (%d)\n", operation.c_str(), 0, y[j], 0, 0, (match_value+penalty_value), match_value, penalty_value);
      //      cost += distance.two_side_distance(0, y[j], 0, 0);
      break;
      
    case Alignment::Type::Contraction:
      match_value = distance.match(x[i-1]+x[i], y[j]);
      operation = "contraction";
      fprintf(stderr, "%s\tdistance(x1=%d, y1=%d, x2=%d, y2=%d) = %d = match_value (%d) + penalty_value (%d)\n", operation.c_str(), x[i-1], y[j], x[i], 0, (match_value+penalty_value), match_value, penalty_value);
      //      cost += distance.two_side_distance(x[i-1], y[j], x[i], 0);
      break;
      
    case Alignment::Type::Expansion:
      match_value = distance.match(x[i], y[j-1]+y[j]);
      operation   = "expansion";
      fprintf(stderr, "%s\tdistance(x1=%d, y1=%d, x2=%d, y2=%d) = %d = match_value (%d) + penalty_value (%d)\n", operation.c_str(), x[i], y[j-1], 0, y[j], (match_value+penalty_value), match_value, penalty_value);
      //      cost += distance.two_side_distance(x[i], y[j-1], 0, y[j]);
      break;
            
    case Alignment::Type::Melding:
      match_value = distance.match(x[i-1]+x[i], y[j-1]+y[j]);
      operation   = "melding";
      fprintf(stderr, "%s\tdistance(x1=%d, y1=%d, x2=%d, y2=%d) = %d = match_value (%d) + penalty_value (%d)\n", operation.c_str(), x[i-1], y[j-1], x[i], y[j], (match_value+penalty_value), match_value, penalty_value);
      //      cost += distance.two_side_distance(x[i-1], y[j-1], x[i], y[j]);
      break;
      
    case Alignment::Type::Invalid:
      return;
      
    }

    cost += penalty_value + match_value;
    
  } 
  
  std::map<Coordinate, Cost>::iterator search = costs.find(current);

  if (search == costs.end() || cost < search->second.cost) {

    costs.emplace(current, Cost(previous, cost));

    //    costs[current] = Cost(previous, cost);
    std::cerr << "New best cost " << cost << " from " << previous << " to " << current << std::endl;

  }
  

}
