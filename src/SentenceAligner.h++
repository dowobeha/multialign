#pragma once

#include <string>
#include <vector>

#include "SentenceAlignments.h++"

class SentenceAligner {

public:

  static void alignPartialDP(std::vector< std::vector<unsigned int> > lengths_all_languages, std::vector<std::string> languages);

  static SentenceAlignments alignFullDP(std::vector< std::vector<unsigned int> > lengths_all_languages, std::vector<std::string> languages);

  static void print(SentenceAlignments alignments, std::vector<std::string> languages);

};
