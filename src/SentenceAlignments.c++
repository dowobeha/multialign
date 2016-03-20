#include "Alignment.h++"
#include "Gale_and_Church_1993.h++"
#include "SentenceAlignments.h++"

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

SentenceAlignments::SentenceAlignments() : values{}, lengths{}, cost{std::numeric_limits<double>::max()} {
  // This space intentionally left blank
}

SentenceAlignments::SentenceAlignments(DynamicProgrammingTable costs, std::map< std::string, std::vector<unsigned int> > lengths) : values{costs.backtrace()}, lengths{lengths}, cost{costs.cost()} {
  // This space intentionally left blank
}

SentenceAlignments::SentenceAlignments(const SentenceAlignments& alignments, const DynamicProgrammingTable& costs, const std::string new_lang, const std::vector<unsigned int>& lengths_new_lang)
: values{SentenceAlignments::merge(alignments, costs)}, lengths{alignments.lengths}, cost{costs.cost()} {
  lengths[new_lang] = lengths_new_lang;
}

/*
 std::vector<int> alignment_of_1_with_2 {1, 2, -1, 3, -1, 4, -1};
 std::vector<int> alignment_of_2_with_1 {1, -1, 2, -1, 3, -1};
 
 WHEN( "further aligned with language 3" ) {
 std::vector<int> alignment_of_3_with_1_and_2 {1, -1, 2, -1};
 std::vector<int> alignment_of_1_and_2_with_3 {1, 2, -1, 3, -1};
 
 std::vector<int> alignment_of_1_with_2_and_3 {1, 2, 3, -1, 4, -1};
 std::vector<int> alignment_of_2_with_1_and_3 {1, 2, -1, 4, -1};
 */

std::vector<int> SentenceAlignments::merge(const std::vector<int>& previouslyAlignedValues,
                                           const std::vector<int>& newlyAlignedValues) {
  
  auto newlyAlignedValuesIterator = newlyAlignedValues.begin();
  const auto& newlyAlignedValuesIteratorEnd = newlyAlignedValues.end();
  
  auto previouslyAlignedValuesIterator = previouslyAlignedValues.begin();
  const auto& previouslyAlignedValuesIteratorEnd = previouslyAlignedValues.end();
  
  std::vector<int> values;
  
  while (newlyAlignedValuesIterator      != newlyAlignedValuesIteratorEnd) {
    
    if (*newlyAlignedValuesIterator < 0) {
      
      values.push_back(*newlyAlignedValuesIterator);
      ++newlyAlignedValuesIterator;
      
    } else {
      
      while (previouslyAlignedValuesIterator != previouslyAlignedValuesIteratorEnd && *previouslyAlignedValuesIterator >= 0) {
        
        values.push_back(*previouslyAlignedValuesIterator);
        ++previouslyAlignedValuesIterator;
        
      }
      
      if (previouslyAlignedValuesIterator != previouslyAlignedValuesIteratorEnd) {
        ++previouslyAlignedValuesIterator;
      }
      
      if (newlyAlignedValuesIterator != newlyAlignedValuesIteratorEnd) {
        ++newlyAlignedValuesIterator;
      }
    }
    
  }
  
  return values;
  
}

std::map< std::string, std::vector< int > > SentenceAlignments::merge(const SentenceAlignments& alignments, const DynamicProgrammingTable& costs) {
  
  std::map< std::string, std::vector< int > > results;
  
  std::map< std::string, std::vector< int > > newlyAlignedBacktrace = costs.backtrace();
  std::string previouslyAlignedLanguages = alignments.languages();
  
  for (const auto& newlyAlignedKeyValue : newlyAlignedBacktrace) {
    
    const auto& newlyAlignedLanguage = newlyAlignedKeyValue.first;
    const auto& newlyAlignedValues   = newlyAlignedKeyValue.second;
    
    if (newlyAlignedLanguage == previouslyAlignedLanguages) {
      
      for (const auto& previouslyAlignedKeyValue : alignments.values) {
        
        const auto& previouslyAlignedLanguage = previouslyAlignedKeyValue.first;
        const auto& previouslyAlignedValues   = previouslyAlignedKeyValue.second;
        
        results[previouslyAlignedLanguage] = merge(previouslyAlignedValues, newlyAlignedValues);
      }
      
      
    } else {
      results[newlyAlignedLanguage] = newlyAlignedValues;
    }
    
  }
  
  return results;
}

bool SentenceAlignments::contains(std::string language) {
  return values.find(language) != values.end();
}

std::string SentenceAlignments::languages() const {
  std::string result = "";
  unsigned int counter = 0;
  unsigned int max = values.size();
  for (auto& keyValue : values) {
    counter += 1;
    result += keyValue.first;
    if (counter < max) {
      result += "-";
    }
  }
  return result;
}

unsigned int SentenceAlignments::numSegments() const {
  unsigned int count = 0;
  
  for (auto value : values.begin()->second) {
    if (value < 0) {
      count += 1;
    }
  }
  
  return count;
}

unsigned int SentenceAlignments::length(const std::string language, const unsigned int previous, const unsigned int current) const {
  
  unsigned int count = 0;
  
  unsigned int segment = 1;
  
  for (const auto value : values.at(language)) {
    
    if (value < 0) {
      segment += 1;
      if (segment > current) {
        break;
      }
    } else if (segment > previous && segment <= current) {
      count += lengths.at(language).at(value);
    }
    
  }
  
  return count;
  
}

double SentenceAlignments::calculateCost(const unsigned int previous, const unsigned int current) const {
  
  //  std::cerr << "*******************************" << std::endl << "calculateCost( previous=" << previous << " , current=" << current << " )" << std::endl;
  
  //  std::cerr << (*this) << std::endl;
  
  //std::cerr << "previous = " << previous << "\tcurrent = " << current << std::endl;
  
  double cost = 0.0;
  
  
  for (const auto& keyValue1 : values) {
    auto l1 = keyValue1.first;
    
    for (const auto& keyValue2 : values) {
      auto l2 = keyValue2.first;
      
      if (l1 < l2) {
        
        unsigned int l1_sum_aligned_segments = 0;
        unsigned int l2_sum_aligned_segments = 0;
        
        for (unsigned int n=previous+1; n<=current; n+=1) {
          l1_sum_aligned_segments += this->numOriginalSegments(l1, n);
          l2_sum_aligned_segments += this->numOriginalSegments(l2, n);
        }
        
        auto alignmentType = Alignment::convert(l1_sum_aligned_segments, l2_sum_aligned_segments);
        
        auto l1_length = this->length(l1, previous, current);
        auto l2_length = this->length(l2, previous, current);
        
        auto penalty = Gale_and_Church_1993::penalty(alignmentType);
        auto match = Gale_and_Church_1993::match(l1_length, l2_length);
        
        //std::cerr << l1 << "-" << l2 << "\t" << alignmentType << "\t" << penalty << "(align: " << l1_sum_aligned_segments << ", " << l2_sum_aligned_segments << ") + " << match << "(match: " << l1_length << " " << l2_length << ")" << std::endl;
        
        cost += penalty + match;
        
      }
    }
  }
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
  
  //  std::cerr << "****************************" << std::endl;
  
  return cost;
  
}

double SentenceAlignments::calculateCost(const unsigned int previous, const unsigned int current, const unsigned int l3_numSegments, const unsigned int l3_length, const std::string l3) const {
  
  double cost = 0.0;
  
  
  for (const auto& keyValue1 : values) {
    auto l1 = keyValue1.first;
    
    unsigned int l1_sum_aligned_segments = 0;
    
    for (unsigned int n=previous+1; n<=current; n+=1) {
      l1_sum_aligned_segments += this->numOriginalSegments(l1, n);
    }
    
    auto alignmentType = Alignment::convert(l1_sum_aligned_segments, l3_numSegments);
    
    auto l1_length = this->length(l1, previous, current);
    
    auto penalty = Gale_and_Church_1993::penalty(alignmentType);
    auto match = Gale_and_Church_1993::match(l1_length, l3_length);
    
    //std::cerr << l1 << "-" << l3 << "\t" << alignmentType << "\t" << penalty << "(align: " << l1_sum_aligned_segments << ", " << l3_numSegments << ") + " << match << "(match: " << l1_length << " " << l3_length << ") l1_length=" << l1_length << " l3_length=" << l3_length << " l1_sum_aligned_segments=" << l1_sum_aligned_segments << " l3_numSegments=" << l3_numSegments << " current=" << current << " previous=" << previous << std::endl;
    
    cost += penalty + match;
    
  }
  
  return cost;
  
}

unsigned int SentenceAlignments::numOriginalSegments(const std::string language, unsigned int alignedSegmentNumber) const {
  
  unsigned int count = 0;
  
  unsigned int currentAlignedSegmentNumber = 1;
  
  for (const auto value : values.at(language)) {
    
    if (value < 0) {
      currentAlignedSegmentNumber += 1;
      if (currentAlignedSegmentNumber > alignedSegmentNumber) {
        break;
      }
    } else if (currentAlignedSegmentNumber == alignedSegmentNumber) {
      count += 1;
    }
    
  }
  
  return count;
}

double SentenceAlignments::getCost() const {
  return cost;
}

void SentenceAlignments::writeToFile(std::ofstream& out, const std::string& language,
                                     std::vector<std::string>& sentences) {
  
  for (auto value : values[language]) {
    
    if (value < 0) {
      out << std::endl;
      std::cerr << std::endl << language << "\t";
    } else {
      //      out <<  paragraphs[language][paragraph_index][value-1];
      out << sentences[value-1];
      out << " ";
      //      std::cerr << paragraphs[language][paragraph_index][value-1] << " ";
      std::cerr << sentences[value-1] << " ";
    }
  }
  
}

std::string SentenceAlignments::alignmentsString() const {

  std::ostringstream os;

  std::map< std::string, std::vector<unsigned int> > counts;
  
  for (auto& keyValue : this->values) {
    counts[keyValue.first] = std::vector<unsigned int>();
    unsigned int counter = 0;
    for (auto& a : keyValue.second) {
      if (a < 0) {
        counts[keyValue.first].push_back(counter);
        counter = 0;
      } else {
        counter += 1;
      }
    }
  }
  
  for (unsigned int j=0, p=counts.begin()->second.size(); j<p; j+=1) {
    unsigned int l = 0;
    unsigned int n = this->values.size();
    for (auto& keyValue : this->values) {
      os << counts[keyValue.first][j];
      if (++l < n) {
        os << ':';
      } else {
        os << std::endl;
      }
    }
  }
  
  return os.str();
}

std::ostream& operator<<(std::ostream& os, const SentenceAlignments& alignments) {
  
  os << std::endl;
  os << std::endl;
  
  
  for (auto& keyValue : alignments.values) {
    os << keyValue.first << "\t";
    for (auto& a : keyValue.second) {
      os << a << " ";
    }
    os << std::endl;
  }
  
  os << alignments.alignmentsString() << std::endl;
  
  return os;
}
