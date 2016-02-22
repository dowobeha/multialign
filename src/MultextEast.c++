#include "MultextEast.h++"
#include "util.h"

#include <iostream>

#include "Coordinates.h++"


MultextEastID::MultextEastID(std::string id) {
  std::istringstream iss(id);
  
  for (std::string token; std::getline(iss, token, '.') && !token.empty(); ) {
    value.push_back(std::stoi(token));
  }

  //std::cerr << "MultextEastID " << id << " : size==" << value.size() << std::endl;
}

bool MultextEastID::operator <(const MultextEastID& that) const {
  return std::lexicographical_compare(value.begin(), value.end(),
				      that.value.begin(), that.value.end());
}


std::ostream& operator<<(std::ostream& os, const MultextEastID& id) {
  //  std::cerr << "Begin MultextEastID << operator" << std::endl;
  auto size = id.value.size();
  //  std::cerr << "size==" << size << std::endl;
  if (size > 0) {
    //    std::cerr << "valid size == " << size << std::endl;
    for (unsigned int index=0; index<size-1; index+=1) {
      os << id.value[index] << '.';
    }
    os << id.value[size-1];
  } else {
    os << "Invalid_ID";
  }
  //  std::cerr << "End MultextEastID << operator" << std::endl;
  return os;
}


const std::string MultextEast::paragraph_pattern = R"(^<p xml:id=\"(.+)\".*)";
const std::regex MultextEast::paragraph_regex{paragraph_pattern};

const std::string MultextEast::sentence_pattern = R"(^<s xml:id=.*$)";
const std::regex MultextEast::sentence_regex{sentence_pattern};


MultextEast::MultextEast(std::string dayfile, std::vector<std::string> languages, std::string dir, std::string outdir) : dayfile(dayfile), languages(languages), dir(dir), outdir(outdir) {

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
    //    std::unique_ptr< std::ofstream, MultextEastFileDeleter > x(p);
    //    std::unique_ptr< std::ofstream > x(p);

    //out.insert(std::make_pair(language, std::unique_ptr< std::ofstream *, std::function<void(std::ofstream *)> >(p, close_ofstream);

    out.insert(std::make_pair( language, std::unique_ptr< std::ofstream, std::function<void(std::ofstream *)> >(new std::ofstream(std::string(outdir + "/" + language + "/" + dayfile).c_str(), std::ofstream::out), close_ofstream)));
  }
  
}


bool MultextEast::skip(std::string language, std::map< std::string, MultextEastID>& map, std::regex pattern, std::string pattern_string) {
  //std::cerr << "skipping in " << language << std::endl;
  auto old_index = index[language];

  bool success = false;
  std::smatch matches;
  for (auto n=txt[language].size(); index[language]<n; index[language]+=1) {
    //std::cerr << "index " << index[language] << " for language " << language << std::endl;
    if (std::regex_match(txt[language][index[language]], matches, pattern)) {
      success = true;
      map[language] = MultextEastID(matches[1]);
      //std::cerr << "Set map["<<language<<"]=="<<map[language]<<std::endl;    
      break;
    }
    //std::cerr << "skip()\tIncrementing index from " << index[language] << " to ";
  }
  //std::cerr << " for language " << language << std::endl;

  //std::cerr << dayfile << " skipped lines " << old_index << "-" << index[language] << " to reach " << pattern_string << std::endl;
  
  return success;
}

bool MultextEast::allIndicesValid() {
  for (auto language : languages) {
    if (index[language] >= size[language]) {
      //std::cerr << "For language " << language << " the current index " << index[language] << " >= " << size[language] << "(size[" << language << "])" << std::endl;
      return false;
    }
  }
  return true;
}


bool MultextEast::advanceIfNeeded(std::string language, std::map< std::string, MultextEastID>& map, std::string pattern, std::regex regex) {
  
  std::smatch matches;

  if (std::regex_match(txt[language][index[language]], matches, regex)) {
    //std::cerr << "Advance was not needed and success == true\tlanguage" << language << " was already at " << txt[language][index[language]] << std::endl;
    map[language] = MultextEastID(matches[1]);
    //std::cerr << "Set map["<<language<<"]=="<<map[language]<<std::endl;
    return true;
  } else {
    bool success = skip(language, map, regex, pattern);
    if (success && index[language] < size[language]) {
      //std::cerr << "Advance was     needed and success == true "<< "\tlanguage " << language << " skipped to " << txt[language][index[language]] << std::endl;
      return true;
    } else {
      //std::cerr << "Advance was     needed and success == false" << std::endl;
      return false;
    }
  }

}

MultextEastID MultextEast::findMaximum(std::map< std::string, MultextEastID>& map) {
  auto max = map[languages[0]];
  //std::cerr << "Value for " << languages[0] << " is " << map[languages[0]] << std::endl;
  for (auto language : languages) {
    //std::cerr << "Value for " << language << " is " << map[language] << std::endl;
    if (max < map[language]) {
      max = map[language];
    }
  }
  //std::cerr << "Value max is " << max << std::endl;
  return max;
 
}

bool MultextEast::match(std::map< std::string, MultextEastID>& map, std::string pattern, std::regex regex) {
  
  map.clear();

  for (auto language : languages) {
    auto success = advanceIfNeeded(language, map, pattern, regex);
    //std::cerr << txt[language][index[language]]<<std::endl;
    //std::cerr << "advanceIfNeeded success (at beginning of match()) == " << success << " for " << pattern << " in language " << language << " with map["<<language<<"]=="<<map[language] <<" and index["<<language<<"]==" << index[language] << "\t" << std::endl;
    if (!success) return false;
  }
    
 here:
  auto max = findMaximum(map);

  for (auto language : languages) {
    while (map[language] < max) {
      auto success = advanceIfNeeded(language, map, pattern, regex);
      //std::cerr << "advanceIfNeeded success (in middle of match()) == " << success << " for " << pattern << " in language " << language << " with max==" << max << " and map["<<language<<"]=="<<map[language] <<" and index["<<language<<"]==" << index[language] << "\t" << txt[language][index[language]]<<std::endl;
      if (success) {
	if (map[language] < max) {
	  index[language] += 1;
	} else if (max < map[language]) {
	  goto here;
	}
      } else {
	return false;
      }
    }
  }
  
  for (auto language : languages) {
    //std::cerr << "Outputting what should be a paragraph tag for " << language << "\t" << txt[language][index[language]] << std::endl;
    //std::cerr << "\t" << txt[language][index[language]] << std::endl;
    *(out[language]) << txt[language][index[language]] << std::endl;
    //std::cerr << "Advancing index for " << language << " from " << index[language];
    index[language] += 1;
    //std::cerr << " to " << index[language] << std::endl;
  }
  
  return true;

}

void MultextEast::align() {
  //std::cerr << "Aligning..." << std::endl;
  txt.clear();
  for (auto language : languages) {
    txt[language] = process("cat " + dir+"/"+language+"/"+dayfile);
    size[language] = txt[language].size();
    //std::cerr << "Language " << language << " has size " << size[language] << std::endl;
  }

  while (true) {
    //std::cerr << "While true..." << std::endl;
    if (allIndicesValid()) {
      //std::cerr << "All indices valid" << std::endl;
      if (anyMatch(paragraph_regex)) {
	//std::cerr << "anyMatch paragraph" << std::endl;
	if (! match(paragraph, paragraph_pattern, paragraph_regex)) {
	  //std::cerr << "! paragraph match" << std::endl;
	  return;
	} else {
	  //std::cerr << "  paragraph match" << std::endl;
	}

      } else {
	//std::cerr << "! anyMatch paragraph" << std::endl;
	if (extractSentences()) {
	  //std::cerr << "Extracted sentences!" << std::endl;
	  for (auto language : languages) {
	    //std::cerr << "Printing out " << language << std::endl;
	    for (auto sentence : sentences[language]) {
	      //std::cerr << "Printing out " << language << "\t:" <<sentence << std::endl;
	      *(out[language]) << sentence << std::endl;
	    }
	  }
	  
	} else { std::cerr << "Failed to extract sentences" << std::endl; }
   
      }

    } else {

      //std::cerr << "Indices not valid" << std::endl;

      return;

    }

  }

}

bool MultextEast::extractSentences() {

  for (auto language : languages) {
    sentences[language].clear();
  }

  for (auto language : languages) {

    for (auto n=size[language]; 
	 index[language]<n && 
	   !std::regex_match(txt[language][index[language]], paragraph_regex);
	 index[language] += 1) {
      
      //std::cerr << "(extractSentences) index " << index[language] << " for language " << language << "\t" << txt[language][index[language]] << "\t" << std::regex_match(txt[language][index[language]], paragraph_regex)  <<std::endl;
      
      //      if (std::regex_match(txt[language][index[language]], sentence_regex) || sentences[language].empty()) {
      //	std::cerr << "Matched sentence_regex for language " << language << std::endl;
	//	sentences[language].push_back( txt[language][index[language]] );
      //      } else {
	//std::cerr << "Before:\tsentences[" << language << "].size == " << sentences[language].size() << std::endl;
            if (! std::regex_match(txt[language][index[language]], sentence_regex)) {
	sentences[language].push_back( txt[language][index[language]] );
	      }
	//std::cerr << "After:\tsentences[" << language << "].size == " << sentences[language].size() << std::endl;
	//      }
	//std::cerr << "extractSentences()\tIncrementing index from " << index[language] << " to ";
    }
    //std::cerr << "index " << index[language] << " for language " << language << std::endl;
  }

  auto expectedSize = sentences[languages[0]].size();
  bool allSameSize = true;
  for (auto language : languages) {
    if (sentences[language].size() != expectedSize) {
      allSameSize = false;
      break;
    }
  }

  if (! allSameSize) {
    std::cerr << dayfile << " (paragraph " << paragraph[languages[0]] << "( different number of sentences ";
    for (auto language : languages) {
      std::cerr << "(" << language << " " << sentences[language].size() << ") ";
    }
    std::cerr << std::endl;
  }

  return allSameSize;
}

bool MultextEast::anyMatch(std::regex regex) {
  for (auto language : languages) {
    if (std::regex_match(txt[language][index[language]], regex)) {
      return true;
    }
  }

  return false;
}
