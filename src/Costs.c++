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
      //      std::cerr << "Previous cost == 0.0 for previous coordinate " << previous << std::endl;
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
      //      fprintf(stderr, "%s\tdistance(x1=%d, y1=%d, x2=%d, y2=%d) = %d = match_value (%d) + penalty_value (%d)", operation.c_str(), x1, y1, x2, y2, (match_value+penalty_value), match_value, penalty_value);    std::cerr << "\t" << previous << " -> " << current << std::endl;
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
      //      std::cerr << "equal\tdistance=0\t" << previous << " -> " << current << std::endl;
      break;
  
    case Alignment::Type::Invalid:
      //      std::cerr << "invalid\tdistance=infinite\t" << previous << " -> " << current << std::endl;
      return;
      
    }

    cost += penalty_value + match_value;
    
  } 
  
  //  std::cerr << "Cost for " << previous << " -> " << current << " should now be " << cost << std::endl;

  std::map<Coordinate, Cost>::iterator search = costs.find(current);

  

  if (search == costs.end()) {

    // Associate the new cost with the current point
    //    std::cerr << "A) Old best cost NONE to " << current << std::endl;
    costs.emplace(current, Cost(previous, cost));
    //    std::cerr << "B) New best cost " << cost << " from " << previous << " to " << current << std::endl;
    //    std::cerr << "C) New best cost " << get(current).cost << " from " << get(current).previous << " to " << current << std::endl;

  } else if ( cost < search->second.cost) {
    //    std::cerr << "A) Old best cost " << get(current).cost << " from " << get(current).previous << " to " << current << std::endl;
    //    std::cout << "W) costs.size()==" << costs.size() << " " << (costs.find(current)==costs.end()) << "\t" << std::endl;
    costs.erase(current);
    //    std::cout << "X) costs.size()==" << costs.size() << " " << (costs.find(current)==costs.end()) << "\t" << std::endl;
    costs.emplace(current, Cost(previous, cost));
    //    std::cout << "Y) costs.size()==" << costs.size() << " " << (costs.find(current)==costs.end()) << "\t" << std::endl;
    //    std::cerr << "B) New best cost " << cost << " from " << previous << " to " << current << std::endl;
    //    std::cerr << "C) New best cost " << get(current).cost << " from " << get(current).previous << " to " << current << std::endl;

  } 
  //else {
    // print out old and new
  //    std::cerr << "A) Old best cost " << get(current).cost << " from " << get(current).previous << " to " << current << std::endl;
  //    std::cerr << "B) New worse cost " << cost << " from " << previous << " to " << current << std::endl;
  //  }

  

}

std::vector< std::vector< int > > Costs::backtrace() const {
//  std::cerr << "Backtrace..." << std::endl;

  std::vector< std::pair<Coordinate,Cost> > backtrace;

  std::function<void(Coordinate)> recursivelyBuildBacktrace = [&](Coordinate c) {
//    std::cerr << "recursivelyBuildBacktrace(" << c << ")" << std::endl;
    auto search = costs.find(c);
    if (search != costs.end()) {
      //backtrace.insert(backtrace.begin(), *search);
      backtrace.push_back(*search);
      recursivelyBuildBacktrace(search->second.previous);
    } 
//    else {
//      std::cerr << "During backtrace, couldn't find " << c << std::endl;
//    }
  };

//  std::cerr << "costs.size()==" << costs.size() << std::endl;

  recursivelyBuildBacktrace(costs.rbegin()->first);
  //  std::reverse(backtrace.begin(),backtrace.end());


//  std::cerr << std::endl << "Backtrace..." << std::endl;

//  for (auto search : backtrace) {
//    auto cost = search.second;
//    std::cerr << cost.previous << " -> " << search.first << " (" << cost.cost << ")" << std::endl;
//  }

//  std::cerr << std::endl;

  //  for (auto search : backtrace) {
  std::vector< std::vector<int> > results;
  for (unsigned int d=0, max=dimensions(); d<max; d+=1) {
    results.push_back(std::vector<int>());
  }

  for (auto i=backtrace.rbegin(), end=backtrace.rend(); i<end; ++i) {
    auto search=*i;
    auto cost = search.second;
    auto current = search.first;
//    std::cerr << cost.previous << " -> " << current << " (" << cost.cost << ")" << std::endl;

    bool noUnalignedSentences=true;
    for (unsigned int d=0, max=dimensions(); d<max; d+=1) {
      if (current.valueAt(d) == cost.previous.valueAt(d)) {
	noUnalignedSentences = false;
	break;
      }
    }

    if (noUnalignedSentences) {
      for (unsigned int d=0, max=dimensions(); d<max; d+=1) {

	std::cout << "Dimension " << d << ":\t";
	for (auto i1=cost.previous.valueAt(d)+1, i2 = current.valueAt(d); i1<=i2; i1+=1) {
	  std::cout << "txt["<<d<<"]["<<i1<<"]" << " ";
	  results[d].push_back(i1);
	}
	results[d].push_back(-1);
	std::cout << std::endl;
      }
    }
  }
  //std::cerr << "(Costs.c++:backtrace()\tDone" << std::endl;
  return results;
}
