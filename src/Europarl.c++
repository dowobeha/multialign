#include "Europarl.h++"
#include "util.h"

#include <iostream>

#include "Coordinates.h++"

const std::string Europarl::chapter_pattern = R"(^<CHAPTER ID=\"?(\d+)\"?.*)";
const std::regex Europarl::chapter_regex{chapter_pattern};
  
const std::string Europarl::speaker_pattern = R"(^<SPEAKER ID=\"?(\d+)\"?.*)";
const std::regex Europarl::speaker_regex{speaker_pattern};

const std::string Europarl::paragraph_pattern = R"(^<P>.*$)";
const std::regex Europarl::paragraph_regex{paragraph_pattern};


Europarl::Europarl(std::string dayfile, std::vector<std::string> languages, std::string dir, std::string outdir, std::string preprocessor) : dayfile(dayfile), languages(languages), dir(dir), outdir(outdir), preprocessor(preprocessor) {

  //  std::function<void(std::ofstream *)> close_ofstream 
  auto close_ofstream = [](std::ofstream *o) -> void { 
    o->close();
    delete o;
  };
  
  for (auto language : languages) {

    //    auto s = std::string(outdir + "/" + dayfile).c_str();
    //    std::ofstream o(s, std::ofstream::out);
    //    std::ofstream *p = &o;

    //    out.insert(std::make_pair(language, 
    //    std::unique_ptr< std::ofstream, EuroparlFileDeleter > x(p);
    //    std::unique_ptr< std::ofstream > x(p);

    //out.insert(std::make_pair(language, std::unique_ptr< std::ofstream *, std::function<void(std::ofstream *)> >(p, close_ofstream);

    out.insert(std::make_pair( language, std::unique_ptr< std::ofstream, std::function<void(std::ofstream *)> >(new std::ofstream(std::string(outdir + "/" + language + "/" + dayfile).c_str(), std::ofstream::out), close_ofstream)));
  }
  
}


bool Europarl::skip(std::string language, std::regex pattern, std::string pattern_string) {
  //std::cerr << "skipping in " << language << std::endl;
  auto old_index = index[language];

  bool success = false;

  for (auto n=txt[language].size(); index[language]<n; index[language]+=1) {
    //std::cerr << "index " << index[language] << " for language " << language << std::endl;
    if (std::regex_match(txt[language][index[language]], pattern)) {
      success = true;
      break;
    }
    //std::cerr << "skip()\tIncrementing index from " << index[language] << " to ";
  }
  //std::cerr << " for language " << language << std::endl;

  //std::cerr << dayfile << " skipped lines " << old_index << "-" << index[language] << " to reach " << pattern_string << std::endl;
  
  return success;
}

bool Europarl::allIndicesValid() {
  for (auto language : languages) {
    if (index[language] >= size[language]) {
      //std::cerr << "For language " << language << " the current index " << index[language] << " >= " << size[language] << "(size[" << language << "])" << std::endl;
      return false;
    }
  }
  return true;
}


bool Europarl::advanceIfNeeded(std::string language, std::map< std::string, unsigned int>& map, std::string pattern, std::regex regex) {
  
  std::smatch matches;

  if (std::regex_match(txt[language][index[language]], matches, regex)) {
    //std::cerr << "Advance was not needed and success == true" << std::endl;
    map[language] = std::stoi(matches[1]);
    return true;
  } else {
    bool success = skip(language, regex, pattern);
    if (success && index[language] < size[language]) {
      //std::cerr << "Advance was     needed and success == true" << std::endl;
      return true;
    } else {
      //std::cerr << "Advance was     needed and success == false" << std::endl;
      return false;
    }
  }

}

unsigned int Europarl::findMaximum(std::map< std::string, unsigned int>& map) {
  auto max = map[languages[0]];

  for (auto language : languages) {
    if (map[language] > max) {
      max = map[language];
    }
  }
  
  return max;
 
}

bool Europarl::match(std::map< std::string, unsigned int>& map, std::string pattern, std::regex regex) {
  
  map.clear();

  for (auto language : languages) {
    auto success = advanceIfNeeded(language, map, pattern, regex);
    //std::cerr << "success == " << success << " for " << pattern << " in language " << language << std::endl;
    if (!success) return false;
  }
    
  auto max = findMaximum(map);

  for (auto language : languages) {
    while (map[language] < max) {
      auto success = advanceIfNeeded(language, map, pattern, regex);
      //std::cerr << "success == " << success << " for " << pattern << " in language " << language << " with max==" << max << " and map["<<language<<"]=="<<map[language] <<" and index["<<language<<"]==" << index[language]<<std::endl;
      if (!success) return false;
    }
  }

  for (auto language : languages) {
    //std::cerr << "\t" << txt[language][index[language]] << std::endl;
    *(out[language]) << txt[language][index[language]] << std::endl;
    //std::cerr << "Advancing index for " << language << " from " << index[language];
    index[language] += 1;
    //std::cerr << " to " << index[language] << std::endl;
  }
    
  return true;

}

void Europarl::align() {

  txt.clear();
  for (auto language : languages) {
    txt[language] = process(preprocessor + " -l " + language + " < " + dir+"/"+language+"/"+dayfile);
    size[language] = txt[language].size();
  }

  while (true) {

    if (allIndicesValid()) {
      if (anyMatch(chapter_regex)) {
		//std::cerr << "Matching chapter..." << std::endl;
	if (! match(chapter, chapter_pattern, chapter_regex)) {
	  return;
	}
      } else if (anyMatch(speaker_regex)) {
		//std::cerr << "Matching speaker..." << std::endl;
	if (! match(speaker, speaker_pattern, speaker_regex)) {
	  return;
	}
      } else {
		//std::cerr << "Matching paragraph..." << std::endl;
	if (extractParagraphs()) {

	  for (unsigned int paragraph_index=0, max=paragraphs[languages[0]].size(); paragraph_index<max; paragraph_index+=1) {
	    std::cerr << "Aligning ";
	    if (!chapter.empty()) { std::cerr << "chapter " << chapter[languages[0]] << " "; }
	    if (!speaker.empty()) { std::cerr << "speaker " << speaker[languages[0]] << " "; }
	    std::cerr << "paragraph " << paragraph_index << std::endl;
	    std::vector< std::vector<unsigned int> > lengths_all_languages;	    
	    for (auto language : languages) {
	      std::vector<unsigned int> lengths_this_language;
	      lengths_this_language.push_back(0);
	      std::cerr << "Lengths for " << language << ":\t[ ";
	      for (auto sentence : paragraphs[language][paragraph_index]) {
		lengths_this_language.push_back(sentence.size());
	      }
	      for (auto length : lengths_this_language) {
		std::cerr << length << " ";
	      }
	      std::cerr << "]" << std::endl;
	      lengths_all_languages.push_back(lengths_this_language);
	    }

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
	    //Coordinates coordinates(gale_and_church);

	    unsigned int counter = 0;

	    std::vector<unsigned int> dimMax = 
	      Coordinates::calculateDimensionalMaxima(lengths_all_languages);


	    Coordinate current(dimMax), previous(dimMax); 

	    std::cerr << "\r" << (++counter) << "\t" << current;
	    do {

	      current.increment();
	      std::cerr << "\r" << (++counter) << "\t" << current;

	      if (previous.resetToEarliestPredecessorOf(current)) {

		do {

		  gale_and_church.calculate(current, previous);

		  previous.increment();

		} while (previous.canIncrement());

	      }
  

	    } while (current.canIncrement());
	    /*
	    for (
		 current.canIncrement(); current.increment()) {
	    
	      //std::cerr << "Current:\t" << current;
	      
	      //	      if (counter > 3000) exit(1);
	      if (previous.resetToEarliestPredecessorOf(current)) {

		//std::cerr << "\tPrevious:\t" << previous << std::endl;

		for ( ; previous.canIncrement(); previous.increment() ) {
		  
       
		  gale_and_church.calculate(current, previous);
		  //		  std::cerr << previous << " -> " << current << std::endl;
		}

	      } else {
		//std::cerr << "\tPrevious:\tNone" << std::endl;
	      }

	    }
	    */
	    std::cerr << std::endl;
	    //	    exit(1);
	    /*
	    for (Coordinate x : coordinates) {
	      //	      if (counter > 1) exit(1);
	      
	      for (Coordinate y : x.possiblePredecessors()) {
		gale_and_church.calculate(x, y);
	      }
	    }
	    std::cout << std::endl;
	    */
	    std::vector< std::vector<std::string> > text;
	    for (auto language : languages) {
	      text.push_back(paragraphs[language][paragraph_index]);
	    }

	    //std::cerr << "gale_and_church.backtrace() begin" << std::endl;
	    auto alignments = gale_and_church.backtrace();
	    //std::cerr << "gale_and_church.backtrace() return" << std::endl;
	    for (unsigned int l=0, n=languages.size(); l<n; l+=1) {
	      for (auto value : alignments[l]) {
		//std::cerr << "\nalignment value for language " << languages[l] << " is " << value << std::endl;
		if (value < 0) {
		  //std::cerr << std::endl;
		  *(out[languages[l]]) << std::endl;
		} else {
		  //		  //std::cerr << "\there" << txt[languages[l]][index[languages[l]]++];
		  //		  *(out[languages[l]]) << txt[languages[l]][index[languages[l]]++];
		  //std::cerr << "\there\t" << paragraphs[languages[l]][paragraph_index][value-1];
		  *(out[languages[l]]) <<  paragraphs[languages[l]][paragraph_index][value-1] << " ";

		}
	      } //std::cerr << std::endl;
	    }
	    //exit(1);
	    //std::cerr << "gale_and_church.backtrace() complete" << std::endl;
	  }
	}

      }
    } else {
      //std::cerr << "Indices not valid..." << std::endl;
      return;
    }

  }

}

bool Europarl::extractParagraphs() {

  for (auto language : languages) {
    paragraphs[language].clear();
  }

  for (auto language : languages) {

    for (auto n=size[language]; 
	 index[language]<n && 
	   !std::regex_match(txt[language][index[language]], chapter_regex) &&
	   !std::regex_match(txt[language][index[language]], speaker_regex);
	 index[language] += 1) {
    //std::cerr << "(extractParagraphs) index " << index[language] << " for language " << language << std::endl;
      if (std::regex_match(txt[language][index[language]], paragraph_regex) || paragraphs[language].empty()) {
	paragraphs[language].push_back(std::vector<std::string>{ txt[language][index[language]] });
      } else {
	//	//std::cerr << "paragraphs[" << language << "].size == " << paragraphs[language].size() << std::endl;
	paragraphs[language].back().push_back(txt[language][index[language]]);
      }
    //std::cerr << "extractParagraphs()\tIncrementing index from " << index[language] << " to ";
    }
    //std::cerr << "index " << index[language] << " for language " << language << std::endl;
  }

  auto expectedSize = paragraphs[languages[0]].size();
  bool allSameSize = true;
  for (auto language : languages) {
    if (paragraphs[language].size() != expectedSize) {
      allSameSize = false;
      break;
    }
  }

  if (! allSameSize) {
    std::cerr << dayfile << " (speaker " << speaker[languages[0]] << "( different number of paragraphs ";
    for (auto language : languages) {
      std::cerr << "(" << language << " " << paragraphs[language].size() << ") ";
    }
    std::cerr << std::endl;
  }

  return allSameSize;
}

bool Europarl::anyMatch(std::regex regex) {
  for (auto language : languages) {
    if (std::regex_match(txt[language][index[language]], regex)) {
      return true;
    }
  }

  return false;
}
