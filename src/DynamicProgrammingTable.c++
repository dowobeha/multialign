#include "DynamicProgrammingTable.h++"

#include <iostream>
#include <functional>
#include <map>
#include <sstream>
#include <vector>

#include "Alignment.h++"
#include "Cost.h++"


unsigned int DynamicProgrammingTable::dimensions() const {
  return languages.size();
}

double DynamicProgrammingTable::get(Coordinate c) const {
    
  auto search = costs.find(c);

  if (search != costs.end()) {
    return search->second.cost;
  } else {
    return 0.0;
  }
  
}

void DynamicProgrammingTable::set(Coordinate current, Coordinate previous, double cost) {

  std::map<Coordinate, Cost>::iterator search = costs.find(current);

  if (search == costs.end()) {

    // Associate the new cost with the current point
    costs.emplace(current, Cost(previous, cost));
    //std::cerr << "\tNever seen  " << current << "\tnew cost =\t" << cost << "  from " << previous << std::endl;

  } else if ( cost < search->second.cost) {

    costs.erase(current);
    costs.emplace(current, Cost(previous, cost));
    //std::cerr << "\tNew cost of " << current << "\t    cost =\t" << cost << "  from " << previous << "  < " << search->second.cost << std::endl;
    
  } else {
    //std::cerr << "\t    cost of " << current << "\t    cost =\t" << cost << "  from " << previous << "  > " << search->second.cost << std::endl;
  }

}

double DynamicProgrammingTable::cost() const {
  return costs.rbegin()->second.cost;
}

std::map< std::string, std::vector< int > > DynamicProgrammingTable::backtrace() const {

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

      //std::cerr << "Dimension " << d << ":\t";
      for (auto i1=cost.previous.valueAt(d)+1, i2 = current.valueAt(d); i1<=i2; i1+=1) {
//	std::cerr << "txt["<<d<<"]["<<i1<<"]" << " ";
	results[languages[d]].push_back(i1);
      }
      results[languages[d]].push_back(-1);
  //    std::cerr << std::endl;
    }
  }
  
  return results;
}

std::ostream& operator<<(std::ostream& os, const DynamicProgrammingTable& t) {

  std::stringstream ss;
  for (auto langItr = t.languages.begin(), end=t.languages.end(); langItr != end; ) {
    ss << *langItr;
    ++langItr;
    if (langItr != end) {
      ss << "-";
    }
  }
  
  for (const auto& keyValue : t.costs) {
    os << ss.str() << "\t" << keyValue.second.previous << " -> " << keyValue.first << "\t" << keyValue.second.cost << std::endl;
  }
  
  
  return os;
}
