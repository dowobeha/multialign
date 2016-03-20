#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <regex>
#include <cmath>
#include <utility>

#include "util.h"

#include "Coordinate.h++"
#include "Coordinates.h++"
#include "Cost.h++"
#include "DynamicProgrammingTable.h++"
#include "Dimensions.h++"
#include "Europarl.h++"
#include "Gale_and_Church_1993.h++"
#include "SentenceAligner.h++"
#include "SentenceAlignments.h++"

using std::string;
using std::vector;
using std::regex;
using std::ofstream;
using std::smatch;
using std::pair;

int main (int argc, char *argv[]) {


/*
  std::vector< std::vector<unsigned int> > lengths_all_languages {
    std::vector<unsigned int> { 0, 66, 215 },
    std::vector<unsigned int> { 0, 59, 217 },
    std::vector<unsigned int> { 0, 73, 238 },
  };
*/
  
/*
 Lengths for bg: [ 0 64 110 149 31 29  81 68 170 100 78 52 ]
 Lengths for cs: [ 0 58 112 110 27 27 138 23 123  90 94 48 ]
 Lengths for en: [ 0 53  94 160 28 30 119 63 159  96 97 57 ]
 
 */
  
  std::vector< std::vector<unsigned int> > lengths_all_languages {
    std::vector<unsigned int> {0, 64, 110, 149, 31, 29, 81, 68, 170, 100, 78, 52},
    std::vector<unsigned int> {0, 58, 112, 110, 27, 27, 138, 23, 123, 90, 94, 48},
    std::vector<unsigned int> {0, 53, 94, 160, 28, 30, 119, 63, 159, 96, 97, 57},
  };

  /*
  std::vector< std::vector<unsigned int> > lengths_all_languages {
    std::vector<unsigned int> {0, 149, 31},
    std::vector<unsigned int> {0, 110, 27},
    std::vector<unsigned int> {0, 160, 28},
  };
  */
  
  std::vector< std::string > languages {
    "bg",
    "cs",
    "en"
  };

  SentenceAlignments bestIncrementalAlignments = SentenceAligner::alignPartialDP(lengths_all_languages, languages);
  
  std::cerr << "Incremental DP alignments:\t" << bestIncrementalAlignments << std::endl;
  
  

  
  std::cerr << std::endl;
  
  SentenceAlignments fullDPAlignments = SentenceAligner::alignFullDP(lengths_all_languages, languages);
  
  std::cerr << "Full DP alignments:\t" << fullDPAlignments << std::endl;
  
  return EXIT_SUCCESS;
}

