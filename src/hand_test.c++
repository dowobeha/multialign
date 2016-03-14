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



  std::vector< std::vector<unsigned int> > lengths_all_languages {
    std::vector<unsigned int> { 0, 66, 215 },
    std::vector<unsigned int> { 0, 59, 217 },
    std::vector<unsigned int> { 0, 73, 238 },
  };

  std::vector< std::string > languages {
    "bg",
    "cs",
    "en"
  };

  /*
  auto dp00 = 0;

  std::cerr << "            dp[0][0]\t=\t" << dp00 << std::endl;

  auto dp01 = 
    // 0-0 match between bg[1] and cs[1]
    ((Gale_and_Church_1993::match(0,0) + Gale_and_Church_1993::penalty(Alignment::Type::Equal)) +
    // 1-1 match between bg[1] and en[1]
     (Gale_and_Church_1993::match(0, 73) + Gale_and_Church_1993::penalty(Alignment::Type::Insertion)) +
    // 1-1 match between cs[1] and en[1]
     (Gale_and_Church_1993::match(0, 73) + Gale_and_Church_1993::penalty(Alignment::Type::Insertion)));

  auto dp00to01 = dp00 + dp01;

  std::cerr << "dp[0][0] -> dp[0][1]\t=\t" << dp00to01 << "\t=\t" << dp00 << " + " << dp01 << std::endl;

  auto dp10 = 
    // 1-1 match between bg[1] and cs[1]
    ((Gale_and_Church_1993::match(66,59) + Gale_and_Church_1993::penalty(Alignment::Type::Substitution)) +
    // 1-1 match between bg[1] and en[1]
     (Gale_and_Church_1993::match(66, 0) + Gale_and_Church_1993::penalty(Alignment::Type::Deletion)) +
    // 1-1 match between cs[1] and en[1]
     (Gale_and_Church_1993::match(59, 0) + Gale_and_Church_1993::penalty(Alignment::Type::Deletion)));

  auto dp00to10 = dp00 + dp10;

  std::cerr << "dp[0][0] -> dp[1][0]\t=\t" << dp00to10 << "\t=\t" << dp00 << " + " << dp10 << std::endl;
  
  auto dp11 = 
    // 1-1 match between bg[1] and cs[1]
    ((Gale_and_Church_1993::match(66, 59) + Gale_and_Church_1993::penalty(Alignment::Type::Substitution)) +
    // 1-1 match between bg[1] and en[1]
		(Gale_and_Church_1993::match(66, 73) + Gale_and_Church_1993::penalty(Alignment::Type::Substitution)) +
    // 1-1 match between cs[1] and en[1]
     (Gale_and_Church_1993::match(59, 73) + Gale_and_Church_1993::penalty(Alignment::Type::Substitution)));

  auto dp00to11 = dp00 + dp11;

  std::cerr << "dp[0][0] -> dp[1][1]\t=\t" << dp00to11 << "\t=\t" << dp00 << " + " << dp11 << std::endl;

*/



  
  SentenceAlignments bestAlignments = SentenceAligner::best2DAlignments(lengths_all_languages, languages);

  std::cerr << "Best 2-d sentence alignments:\t" << bestAlignments << std::endl;

  unsigned int segmentsInBestAlignment = bestAlignments.numSegments();
  

  for (unsigned int l3=0, n=lengths_all_languages.size(); l3<n; l3+=1) {
    if (bestAlignments.contains(languages[l3])) {
      std::cerr << languages[l3] << "\tskipping" << std::endl;
    } else {
      std::cerr << languages[l3] << "\tnot skipping" << std::endl;

      // Record the number of segments in each of the two versions of the corpus to be aligned next
      std::vector<unsigned int> dimMax { 
	// the number of segments in the previously aligned data
	segmentsInBestAlignment, 
	// the number of segments in l3
	(unsigned int) lengths_all_languages[l3].size()-1 
      };

      // Initialize 2-dimensional coordinates
      Coordinate current(dimMax), previous(dimMax);

      // Record the languages now being aligned
      std::vector<std::string> currentLanguages{bestAlignments.languages(), languages[l3]};

      // Construct an empty 2-dimensional dynamic programming table
      DynamicProgrammingTable dynamicProgrammingTable(currentLanguages);

      // Incrementally fill in values of dynamic programmic table
      do {

	current.increment();
	std::cerr << languages[l3] << "               " << current << std::endl;

	if (previous.resetToEarliestPredecessorOf(current)) {

	  do {


	    
	    //	    SentenceAlignments::numOriginalSegments(

	    double cost = dynamicProgrammingTable.get(previous);

	    std::pair<unsigned int, unsigned int> dimensions{0,1};


	    // Calculate the alignment between 
	    //
	    //  - elements (tuples of previously aligned sentences) 
	    //      in the existing best alignment (of the previously aligned languages)
	    //
	    //  - sentences in language l3
	    //
	    Alignment::Type alignment = 
	      Alignment::determine(current.valueAt(dimensions.first),  current.valueAt(dimensions.second),
				   previous.valueAt(dimensions.first), previous.valueAt(dimensions.second));

	    if (alignment != Alignment::Type::Invalid) {

	      std::cerr << languages[l3] << "\t" << previous << " -> " << current << "\t" << alignment << std::endl;


	      unsigned int l3_numSegments = 
		current.valueAt(dimensions.second) - 
		previous.valueAt(dimensions.second);

	      unsigned int l3_length = 0;
	      for (unsigned int n=previous.valueAt(dimensions.second)+1, n_max=current.valueAt(dimensions.second); n<=n_max; n+=1) {
		l3_length += lengths_all_languages[l3][n];
	      } 
	      

	      auto c = bestAlignments.calculateCost(previous.valueAt(dimensions.first), current.valueAt(dimensions.first)) + bestAlignments.calculateCost(previous.valueAt(dimensions.first), current.valueAt(dimensions.first), l3_numSegments, l3_length, languages[l3]);

	      std::cerr << bestAlignments.languages() << "-" << languages[l3] << " cost\t=\t" << c << std::endl;

	      cost += c;


	      /*
	      for (auto language : languages) {
		if (bestAlignments.contains(language)) {
		  unsigned int sumAlreadyAlignedSegments = 0;
		  for (unsigned int n=previous.valueAt(dimensions.first), n_max=current.valueAt(dimensions.first); n<=n_max; n+=1) {
		    auto numAlreadyAlignedSegments = bestAlignments.numOriginalSegments(language, n);
		    sumAlreadyAlignedSegments += numAlreadyAlignedSegments;
		    std::cerr << "numOriginalSegments(" << language <<", "<<n<<")\t=\t" << numAlreadyAlignedSegments << std::endl;
		  }
		  std::cerr << language << "\t" << sumAlreadyAlignedSegments << " segments" << std::endl;
		}
	      }
	      */

	      /*
	      int penalty_value = Gale_and_Church_1993::penalty(alignment);
	    
	      //TODO: Beginning here, this block of code needs to be reworked in order to correctly implement Simard (1999)
	      int match_value = Gale_and_Church_1993::twoDimensionalMatchCost(alignment,
									      current.valueAt(dimensions.first), 
									      lengths_all_languages[dimensions.first],
									      current.valueAt(dimensions.second),
									      lengths_all_languages[dimensions.second]);
	    
	      cost += penalty_value + match_value;
	      */
	      //TODO: Ending here, this block of code needs to be reworked in order to correctly implement Simard (1999)
	    
	   
  
	      dynamicProgrammingTable.set(current, previous, cost);

	    }
	    previous.increment();

	  } while (previous.canIncrement());

	}
  
      } while (current.canIncrement());



    }
  }
  
  return EXIT_SUCCESS;
}

