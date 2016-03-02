#include <iostream>
#include <vector>

#include "Costs.h++"
#include "Coordinate.h++"
#include "Coordinates.h++"
#include "Dimensions.h++"
#include "SentenceAligner.h++"
#include "SentenceAlignments.h++"


void SentenceAligner::alignPartialDP(std::vector< std::vector<unsigned int> > lengths_all_languages, std::vector<std::string> languages) {

  SentenceAlignments bestAlignments;

  for (unsigned int l1=0, n=lengths_all_languages.size(); l1<n; l1+=1) {
    for (unsigned int l2=l1+1; l2<n; l2+=1) {

      auto alignments = 
	SentenceAligner::alignFullDP(
	    std::vector< std::vector<unsigned int > >{
	      lengths_all_languages[l1],
	      lengths_all_languages[l2]},
	    std::vector<std::string>{
	      languages[l1],
	      languages[l2]}
	    );

      std::cerr << "2-dimensional alignment for languages " << languages[l1] << " and " << languages[l2] << " has cost " << alignments.cost << std::endl;

      if (alignments.cost < bestAlignments.cost) {
	bestAlignments = alignments;
      }

    }
  }

  std::cerr << "Best 2-dimensional alignment is for languages ( ";
  for (const auto &keyValue : bestAlignments.values) {
    std::cerr << keyValue.first << " ";
  }
  std::cerr << ") with cost " << bestAlignments.cost << std::endl;

  unsigned int segmentsInBestAlignment = bestAlignments.numSegments();
  

  for (unsigned int l3=0, n=lengths_all_languages.size(); l3<n; l3+=1) {
    if (! bestAlignments.contains(languages[l3])) {
      std::vector<unsigned int> dimMax { segmentsInBestAlignment, (unsigned int) lengths_all_languages[l3].size()-1 };
      Coordinate current(dimMax), previous(dimMax);

      do {

	current.increment();
	
	if (previous.resetToEarliestPredecessorOf(current)) {

	  do {

	    //	    gale_and_church.calculate(current, previous);

	    previous.increment();

	  } while (previous.canIncrement());

	}
  
      } while (current.canIncrement());



    }
  }

}

SentenceAlignments SentenceAligner::alignFullDP(std::vector< std::vector<unsigned int> > lengths_all_languages, std::vector<std::string> languages) {

  {
    unsigned int total = 1;
    std::cerr << "Total number of coordinates to explore = \\prod_{i in";
    for (auto lengths : lengths_all_languages) {
      total *= lengths.size();
      std::cerr << " " << lengths.size();
    }
    std::cerr << "} = " << total << std::endl;
  }

  Costs gale_and_church(languages);


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

	double cost = gale_and_church.get(previous);


	for (std::pair<unsigned int, unsigned int> dimensions : Dimensions(gale_and_church.dimensions())) {
            
	  Alignment::Type alignment = 
	    Alignment::determine(current.valueAt(dimensions.first),  current.valueAt(dimensions.second),
				 previous.valueAt(dimensions.first), previous.valueAt(dimensions.second));

	  int penalty_value = Distance::penalty(alignment);

	  int match_value = gale_and_church.twoDimensionalMatchCost(alignment,
						    current.valueAt(dimensions.first), 
						    lengths_all_languages[dimensions.first],
						    current.valueAt(dimensions.second),
						    lengths_all_languages[dimensions.second]);

	  cost += penalty_value + match_value;
    
	} 
  
	gale_and_church.set(current, previous, cost);


	previous.increment();

      } while (previous.canIncrement());

    }
  

  } while (current.canIncrement());

  return SentenceAlignments{gale_and_church};

}

void SentenceAligner::print(SentenceAlignments alignments, std::vector<std::string> languages) {

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
    for (unsigned int a=0, m=alignments.values[languages[l]].size(); a<m; a+=1) {
      std::cerr << alignments.values[languages[l]][a] << " ";
    }
    std::cerr << std::endl;
  }
  std::vector< std::vector<unsigned int> > counts;
  for (unsigned int l=0, n=languages.size(); l<n; l+=1) {
    counts.push_back(std::vector<unsigned int>());
    unsigned int counter = 0;
    for (unsigned int a=0, m=alignments.values[languages[l]].size(); a<m; a+=1) {
      if (alignments.values[languages[l]][a] < 0) {
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
