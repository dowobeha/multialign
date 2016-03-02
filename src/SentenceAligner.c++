#include <iostream>
#include <vector>

#include "Costs.h++"
#include "Coordinate.h++"
#include "Coordinates.h++"
#include "SentenceAligner.h++"

std::vector< std::vector< int > > SentenceAligner::alignFullDP(std::vector< std::vector<unsigned int> > lengths_all_languages) {

  {
    unsigned int total = 1;
    std::cerr << "Total number of coordinates to explore = \\prod_{i in";
    for (auto lengths : lengths_all_languages) {
      total *= lengths.size();
      std::cerr << " " << lengths.size();
    }
    std::cerr << "} = " << total << std::endl;
  }

  Costs gale_and_church(lengths_all_languages);


  unsigned int counter = 0;

  std::vector<unsigned int> dimMax = 
    Coordinates::calculateDimensionalMaxima(lengths_all_languages);


  Coordinate current(dimMax), previous(dimMax); 

  std::cerr << "\r" << (++counter) << "\t" << current;
  do {
    //	      std::cerr << "Incrementing current from " << current;
    current.increment();
    //	      std::cerr << " to " << current << std::endl;
    std::cerr << "\r" << (++counter) << "\t" << current;

    if (previous.resetToEarliestPredecessorOf(current)) {

      do {

	gale_and_church.calculate(current, previous);

	previous.increment();

      } while (previous.canIncrement());

    }
  

  } while (current.canIncrement());

  return gale_and_church.backtrace();

}

void SentenceAligner::print(std::vector< std::vector< int > > alignments, std::vector<std::string> languages) {

  std::cerr << std::endl;
  std::cerr << std::endl;

  /*
    std::vector< std::vector<std::string> > text;
    for (auto language : languages) {
    text.push_back(paragraphs[language][paragraph_index]);
    }
  */

  //std::cerr << "gale_and_church.backtrace() begin" << std::endl;

  for (unsigned int l=0, n=languages.size(); l<n; l+=1) {
    std::cerr << languages[l] << "\t";
    for (unsigned int a=0, m=alignments[l].size(); a<m; a+=1) {
      std::cerr << alignments[l][a] << " ";
    }
    std::cerr << std::endl;
  }
  std::vector< std::vector<unsigned int> > counts;
  for (unsigned int l=0, n=languages.size(); l<n; l+=1) {
    counts.push_back(std::vector<unsigned int>());
    unsigned int counter = 0;
    for (unsigned int a=0, m=alignments[l].size(); a<m; a+=1) {
      if (alignments[l][a] < 0) {
	counts[l].push_back(counter);
	counter = 0;
      } else {
	counter += 1;
      }
    }
  }
  for (unsigned int j=0, p=counts[0].size(); j<p; j+=1) {
    for (unsigned int l=0, n=languages.size(); l<n; l+=1) {
      std::cout << counts[l][j];
      if (l+1<n) {
	std::cout << ':';
      } else {
	std::cout << std::endl;
      }
    }
  }


}
