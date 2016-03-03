#pragma once

#include <fstream>
#include <ostream>
#include <limits>
#include <string>
#include <vector>

#include "DynamicProgrammingTable.h++"

class SentenceAlignments {

private:

  std::map< std::string, std::vector< int > > values;

  double cost;

  
public:


  SentenceAlignments();

  SentenceAlignments(DynamicProgrammingTable costs);

  bool contains(std::string language);

  std::string languages() const;

  unsigned int numSegments() const;

  double getCost() const;

  friend std::ostream& operator<<(std::ostream& os, const SentenceAlignments& s);

  void writeToFile(std::ofstream& ofs, const std::string& language, std::vector<std::string>& sentences);

};
