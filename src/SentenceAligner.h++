#pragma once

#include <vector>


class SentenceAligner {

public:

  static std::vector< std::vector< int > > alignFullDP(std::vector< std::vector<unsigned int> > lengths_all_languages);

  static void print(std::vector< std::vector< int > > alignments, std::vector<std::string> languages);

};
