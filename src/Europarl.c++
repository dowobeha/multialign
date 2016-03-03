#include "Europarl.h++"
#include "util.h"

#include <iostream>

#include "Coordinates.h++"
#include "SentenceAligner.h++"

#include "util.h"

const std::string Europarl::chapter_pattern = R"(^<CHAPTER ID=\"?(\d+)\"?.*)";
const std::regex Europarl::chapter_regex{chapter_pattern};
  
const std::string Europarl::speaker_pattern = R"(^<SPEAKER ID=\"?(\d+)\"?.*)";
const std::regex Europarl::speaker_regex{speaker_pattern};

const std::string Europarl::paragraph_pattern = R"(^<P>.*$)";
const std::regex Europarl::paragraph_regex{paragraph_pattern};


Europarl::Europarl(std::string dayfile, 
		   std::vector<std::string> languages, 
		   std::string dir, 
		   std::string outdir, 
		   std::string preprocessor) : dayfile(dayfile), 
					       languages(languages), 
					       dir(dir), 
					       outdir(outdir), 
					       preprocessor(preprocessor) {

  // construct a custom deleter for a file output stream
  // 
  // this custom function is responsible for closing the file output stream and deleting the object
  auto close_ofstream = [](std::ofstream *o) -> void { 
    o->close();
    delete o;
  };
  
  for (auto language : languages) {

    // out is a map from language to file output stream
    //
    // the following code inserts a new key-value pair into the map
    out.insert(
	       // explicitly create the key-value pair
	       std::make_pair(
			      // language is the key
			      language,
			      // a file output stream is the value
			      //
			      // wrap the output stream in a unique pointer
			      std::unique_ptr< std::ofstream,
					       // and specify a custom deleter for the output stream
						std::function<void(std::ofstream *)> >(
     // manually allocate a new file output stream
     new std::ofstream(
		       // specify the file path
		       std::string(outdir + "/" + language + "/" + dayfile).c_str(), 
		       // specify that we want to overwrite the file if it exists
		       std::ofstream::out
		      ),
     // specify the custom deleter for the file output stream object 
     //
     // this custom deleter function will be invoked automatically when needed
     close_ofstream)));
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

  return success;
}


bool Europarl::allIndicesValid() {
  for (auto language : languages) {
    if (index[language] >= size[language]) {
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
    if (!success) return false;
  }
    
  auto max = findMaximum(map);

  for (auto language : languages) {
    while (map[language] < max) {
      auto success = advanceIfNeeded(language, map, pattern, regex);
      if (!success) return false;
    }
  }

  for (auto language : languages) {
    if (language=="en") std::cout << txt[language][index[language]] << std::endl;
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
		lengths_this_language.push_back(utf8_codepoints(sentence));
	      }
	      for (auto length : lengths_this_language) {
		std::cerr << length << " ";
	      }
	      std::cerr << "]" << std::endl;
	      lengths_all_languages.push_back(lengths_this_language);
	    }

	    SentenceAligner::alignPartialDP(lengths_all_languages, languages);

	    auto alignments = SentenceAligner::alignFullDP(lengths_all_languages, languages);
	    std::cerr << alignments;

	    for (unsigned int l=0, n=languages.size(); l<n; l+=1) {

	      std::cerr << languages[l] << "\t";

	      alignments.writeToFile(*(out[languages[l]]), languages[l],
				     paragraphs[languages[l]][paragraph_index]);

	      std::cerr << std::endl;

	    }

	  }
	}

      }
    } else {
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

      if (std::regex_match(txt[language][index[language]], paragraph_regex) || paragraphs[language].empty()) {

	paragraphs[language].push_back(std::vector<std::string>());
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
