#pragma once

#include <fstream>
#include <ostream>
#include <limits>
#include <map>
#include <string>
#include <vector>

#include "DynamicProgrammingTable.h++"

class SentenceAlignments {

private:

  std::map< std::string, std::vector< int > > values;

  std::map< std::string, std::vector<unsigned int> > lengths;

  double cost;

  static std::map< std::string, std::vector< int > > merge(const SentenceAlignments& alignments, const DynamicProgrammingTable& costs);

public:

  static std::vector<int> merge(const std::vector<int>& previouslyAlignedValues, const std::vector<int>& newlyAlignedValues);

  SentenceAlignments();

  SentenceAlignments(DynamicProgrammingTable costs, std::map< std::string, std::vector<unsigned int> > lang_to_lengths);
  
  SentenceAlignments(const SentenceAlignments& alignments, const DynamicProgrammingTable& costs, const std::string new_lang, const std::vector<unsigned int>& lengths_new_lang);

  bool contains(std::string language);

  std::string languages() const;

  unsigned int numSegments() const;

  unsigned int numOriginalSegments(std::string language, unsigned int alignedSegmentNumber) const;

  unsigned int length(const std::string language, const unsigned int previous, const unsigned int current) const;

  double calculateCost(const unsigned int previous, const unsigned int current) const;

  double calculateCost(const unsigned int previous, const unsigned int current, const unsigned int l3_numSegments, const unsigned int l3_length, const std::string l3) const;

  double getCost() const;

  std::string alignmentsString() const;

  friend std::ostream& operator<<(std::ostream& os, const SentenceAlignments& s);

  void writeToFile(std::ofstream& ofs, const std::string& language, std::vector<std::string>& sentences);

};
