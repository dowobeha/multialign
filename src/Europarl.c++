#include "Europarl.h++"
#include "util.h"

#include <iostream>

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
  
  auto old_index = index[language];

  bool success = false;

  for (auto n=txt[language].size(); index[language]<n; index[language]+=1) {
    if (std::regex_match(txt[language][index[language]], pattern)) {
      success = true;
      break;
    }
  }

  std::cerr << dayfile << " skipped lines " << old_index << "-" << index[language] << " to reach " << pattern_string << std::endl;
  
  return success;
}

bool Europarl::allIndicesValid() {
  for (auto language : languages) {
    if (index[language] >= size[language]) {
      std::cerr << "For language " << language << " the current index " << index[language] << " >= " << size[language] << "(size[" << language << "])" << std::endl;
      return false;
    }
  }
  return true;
}


bool Europarl::advanceIfNeeded(std::string language, std::map< std::string, unsigned int>& map, std::string pattern, std::regex regex) {
  
  std::smatch matches;

  if (std::regex_match(txt[language][index[language]], matches, regex)) {
    map[language] = std::stoi(matches[1]);
    return true;
  } else {
    bool success = skip(language, regex, pattern);
    if (success && index[language] < size[language]) {
      return true;
    } else {
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
    std::cerr << "success == " << success << " for " << pattern << " in language " << language << std::endl;
    if (!success) return false;
  }
    
  auto max = findMaximum(map);

  for (auto language : languages) {
    while (map[language] < max) {
      auto success = advanceIfNeeded(language, map, pattern, regex);
      std::cerr << "success == " << success << " for " << pattern << " in language " << language << " with max==" << max << std::endl;
      if (!success) return false;
    }
  }

  for (auto language : languages) {
    *(out[language]) << txt[language][index[language]] << std::endl;
    index[language] += 1;
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
	std::cerr << "Matching chapter..." << std::endl;
	if (! match(chapter, chapter_pattern, chapter_regex)) {
	  return;
	}
      } else if (anyMatch(speaker_regex)) {
	std::cerr << "Matching speaker..." << std::endl;
	if (! match(speaker, speaker_pattern, speaker_regex)) {
	  return;
	}
      } else {
	std::cerr << "Matching paragraph..." << std::endl;
	if (extractParagraphs()) {
	  // TODO: more here
	}
      }
    } else {
      std::cerr << "Indices not valid..." << std::endl;
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

      if (std::regex_match(txt[language][index[language]], paragraph_regex) || paragraphs.empty()) {
	paragraphs[language].push_back(std::vector<std::string>{ txt[language][index[language]] });
      } else {
	paragraphs[language].back().push_back(txt[language][index[language]]);
      }

    }

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
