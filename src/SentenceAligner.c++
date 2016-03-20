#include <iostream>
#include <vector>

#include "Alignment.h++"
#include "DynamicProgrammingTable.h++"
#include "Coordinate.h++"
#include "Coordinates.h++"
#include "Dimensions.h++"
#include "Gale_and_Church_1993.h++"
#include "SentenceAligner.h++"
#include "SentenceAlignments.h++"

SentenceAlignments SentenceAligner::best2DAlignments(std::vector< std::vector<unsigned int> > lengths_all_languages, std::vector<std::string> languages) {
  
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
      
      std::cerr << "2-dimensional alignment for languages " << languages[l1] << " and " << languages[l2] << " has cost " << alignments.getCost() << std::endl;
      
      if (alignments.getCost() < bestAlignments.getCost()) {
        bestAlignments = alignments;
      }
      
    }
  }
  
  std::cerr << "Best 2-dimensional alignment is for languages ( "
  << bestAlignments.languages()
  << ") with cost "
  << bestAlignments.getCost()
  << std::endl;
  
  return bestAlignments;
  
}

SentenceAlignments SentenceAligner::alignPartialDP(std::vector< std::vector<unsigned int> > lengths_all_languages, std::vector<std::string> languages) {
  
  SentenceAlignments bestAlignments = SentenceAligner::best2DAlignments(lengths_all_languages, languages);
  
  //std::cerr << "Best 2-d sentence alignments:\t" << bestAlignments << std::endl;
  
  unsigned int segmentsInBestAlignment = bestAlignments.numSegments();
  
  
  for (unsigned int l3=0, n=lengths_all_languages.size(); l3<n; l3+=1) {
    if (bestAlignments.contains(languages[l3])) {
      //std::cerr << languages[l3] << "\tskipping" << std::endl;
    } else {
      //std::cerr << languages[l3] << "\tnot skipping" << std::endl;
      
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
        //std::cerr << languages[l3] << "               " << current << std::endl;
        
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
              
              //std::cerr << languages[l3] << "\t" << previous << " -> " << current << "\t" << alignment << std::endl;
              
              
              unsigned int l3_numSegments =
              current.valueAt(dimensions.second) -
              previous.valueAt(dimensions.second);
              
              unsigned int l3_length = 0;
              for (unsigned int n=previous.valueAt(dimensions.second)+1, n_max=current.valueAt(dimensions.second); n<=n_max; n+=1) {
                l3_length += lengths_all_languages[l3][n];
              }
              //std::cerr << "Calculating cost of best alignments" << std::endl;
              auto a = bestAlignments.calculateCost(previous.valueAt(dimensions.first), current.valueAt(dimensions.first));
              
              //std::cerr << "\t" << bestAlignments.languages() << "-" << languages[l3] << "\t" << previous << " -> " << current << std::endl;
              auto b = bestAlignments.calculateCost(previous.valueAt(dimensions.first), current.valueAt(dimensions.first), l3_numSegments, l3_length, languages[l3]);
              
              auto c = a + b;
              
              //std::cerr << "New possible alignment:\t" << bestAlignments.languages() << "-" << languages[l3] << "\t" << previous << " -> " << current << "\t" << "=\t" << c << "\t=\t" << a << " + " << b << std::endl;
              
              cost += c;
              
              dynamicProgrammingTable.set(current, previous, cost);
              
            }
            previous.increment();
            
          } while (previous.canIncrement());
          
        }
        
      } while (current.canIncrement());
      
      //std::cerr << dynamicProgrammingTable << std::endl;
      
      /*
       auto bt = dynamicProgrammingTable.backtrace();
       for (const auto& keyValue : bt) {
       std::cerr << "Backtrace:\t" << keyValue.first << "\t";
       for (const auto& v : keyValue.second) {
       std::cerr << v << " ";
       }
       std::cerr << std::endl;
       }
       */
      std::cerr << "Updating bestAlignments by adding " << languages[l3] << " to achieve ";
      bestAlignments = SentenceAlignments(bestAlignments, dynamicProgrammingTable, languages[l3], lengths_all_languages[l3]);
      std::cerr << bestAlignments.languages() << std::endl;
      
      //std::cerr << "Updated best sentence alignments:\t" << bestAlignments << std::endl;
      
    }
  }
  
  return bestAlignments;
  
  /*
   SentenceAlignments bestAlignments = best2DAlignments(lengths_all_languages, languages);
   
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
   
   int penalty_value = Gale_and_Church_1993::penalty(alignment);
   
   //TODO: Beginning here, this block of code needs to be reworked in order to correctly implement Simard (1999)
   int match_value = Gale_and_Church_1993::twoDimensionalMatchCost(alignment,
   current.valueAt(dimensions.first),
   lengths_all_languages[dimensions.first],
   current.valueAt(dimensions.second),
   lengths_all_languages[dimensions.second]);
   
   cost += penalty_value + match_value;
   //TODO: Ending here, this block of code needs to be reworked in order to correctly implement Simard (1999)
   
   
   
   dynamicProgrammingTable.set(current, previous, cost);
   
   }
   previous.increment();
   
	  } while (previous.canIncrement());
   
   }
   
   } while (current.canIncrement());
   
   
   
   }
   }
   */
}

SentenceAlignments SentenceAligner::alignFullDP(std::vector< std::vector<unsigned int> > lengths_all_languages, std::vector<std::string> languages) {
  /*
   {
   unsigned int total = 1;
   std::cerr << "Total number of coordinates to explore = \\prod_{i in";
   for (auto lengths : lengths_all_languages) {
   total *= lengths.size();
   std::cerr << " " << lengths.size();
   }
   std::cerr << "} = " << total << std::endl;
   }
   unsigned int counter = 0;
   */
  
  DynamicProgrammingTable dynamicProgrammingTable(languages);
  
  
  
  
  std::vector<unsigned int> dimMax =
  Coordinates::calculateDimensionalMaxima(lengths_all_languages);
  
  
  Coordinate current(dimMax), previous(dimMax);
  
  //std::cerr << "\r" << (++counter) << "\t" << current;
  do {
    //	      std::cerr << "Incrementing current from " << current;
    current.increment();
    //	      std::cerr << " to " << current << std::endl;
    //std::cerr << "\r" << (++counter) << "\t" << current;
    
    if (previous.resetToEarliestPredecessorOf(current)) {
      
      do {
        
        double cost = dynamicProgrammingTable.get(previous);
        
        
        for (std::pair<unsigned int, unsigned int> dimensions : Dimensions(dynamicProgrammingTable.dimensions())) {
          
          Alignment::Type alignment =
          Alignment::determine(current.valueAt(dimensions.first),  current.valueAt(dimensions.second),
                               previous.valueAt(dimensions.first), previous.valueAt(dimensions.second));
          
          int penalty_value = Gale_and_Church_1993::penalty(alignment);
          
          int match_value = Gale_and_Church_1993::twoDimensionalMatchCost(alignment,
                                                                          current.valueAt(dimensions.first),
                                                                          lengths_all_languages[dimensions.first],
                                                                          current.valueAt(dimensions.second),
                                                                          lengths_all_languages[dimensions.second]);
          
          cost += penalty_value + match_value;
          
        } 
        
        dynamicProgrammingTable.set(current, previous, cost);
        
        
        previous.increment();
        
      } while (previous.canIncrement());
      
    }
    
    
  } while (current.canIncrement());
  
  std::map< std::string, std::vector<unsigned int> > lang_to_lengths;
  for (unsigned int l=0, n=languages.size(); l<n; l+=1) {
    lang_to_lengths[languages[l]] = lengths_all_languages[l];
  }
  
  //std::cerr << dynamicProgrammingTable << std::endl;
  
  return SentenceAlignments{dynamicProgrammingTable, lang_to_lengths};
  
}
