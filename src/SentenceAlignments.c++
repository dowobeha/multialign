#include "SentenceAlignments.h++"

#include <iostream>
#include <map>
#include <string>
#include <vector>

SentenceAlignments::SentenceAlignments() : values{}, cost{std::numeric_limits<double>::max()} {
  // This space intentionally left blank
}

SentenceAlignments::SentenceAlignments(DynamicProgrammingTable costs) : values{costs.backtrace()}, cost{costs.cost()} {
  // This space intentionally left blank
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

  std::map< std::string, std::vector<unsigned int> > counts;

  for (auto& keyValue : alignments.values) {
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
    unsigned int n = alignments.values.size();
    for (auto& keyValue : alignments.values) {
      os << counts[keyValue.first][j];
      if (++l < n) {
	os << ':';
      } else {
	os << std::endl;
      }
    }
  }

  return os;
}
