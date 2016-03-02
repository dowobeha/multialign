#include "Costs.h++"

#include <iostream>
#include <functional>
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

  double cost; {
    auto search = costs.find(previous);
    if (search != costs.end()) {
      cost = search->second.cost;
    } else {
      cost = 0.0;
    }
  }


  for (std::pair<unsigned int, unsigned int> dimensions : Dimensions(this->dimensions())) {
    
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
      operation   = "merger";
      match(x[i-1], y[j-1], x[i], y[j]);
      break;

    case Alignment::Type::Equal:
      operation   = "equal";
      match_value = 0;
      break;
  
    case Alignment::Type::Invalid:
      return;
      
    }

    cost += penalty_value + match_value;
    
  } 
  

  std::map<Coordinate, Cost>::iterator search = costs.find(current);


  if (search == costs.end()) {

    // Associate the new cost with the current point
    costs.emplace(current, Cost(previous, cost));

  } else if ( cost < search->second.cost) {

    costs.erase(current);
    costs.emplace(current, Cost(previous, cost));

  } 

}

double Costs::cost() const {
  return costs.rbegin()->second.cost;
}

std::map< std::string, std::vector< int > > Costs::backtrace() const {

  std::vector< std::pair<Coordinate,Cost> > backtrace;

  std::function<void(Coordinate)> recursivelyBuildBacktrace = [&](Coordinate c) {

    auto search = costs.find(c);
    if (search != costs.end()) {

      backtrace.push_back(*search);
      recursivelyBuildBacktrace(search->second.previous);
    } 

  };


  recursivelyBuildBacktrace(costs.rbegin()->first);

  //  std::vector< std::vector<int> > results;
  std::map< std::string, std::vector<int> > results;
  for (unsigned int d=0, max=dimensions(); d<max; d+=1) {
    results[languages[d]] = std::vector<int>();
  }

  for (auto i=backtrace.rbegin(), end=backtrace.rend(); i<end; ++i) {
    auto search=*i;
    auto cost = search.second;
    auto current = search.first;

    for (unsigned int d=0, max=dimensions(); d<max; d+=1) {

      std::cerr << "Dimension " << d << ":\t";
      for (auto i1=cost.previous.valueAt(d)+1, i2 = current.valueAt(d); i1<=i2; i1+=1) {
	std::cerr << "txt["<<d<<"]["<<i1<<"]" << " ";
	results[languages[d]].push_back(i1);
      }
      results[languages[d]].push_back(-1);
      std::cerr << std::endl;
    }
  }
  
  return results;
}
