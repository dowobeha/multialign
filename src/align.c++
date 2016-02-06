#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <regex>
#include <cmath>
#include <utility>

#include "util.h"

#include "Coordinate.h++"
#include "CoordinateIterator.h++"
#include "Coordinates.h++"
#include "Cost.h++"
#include "Costs.h++"
#include "Dimensions.h++"
#include "Europarl.h++"

using std::string;
using std::vector;
using std::regex;
using std::ofstream;
using std::smatch;
using std::pair;
/*
unsigned int skip(string dayfile, vector<string>& txt, unsigned int i, regex pattern, string pattern_string) {
  
  auto old_i = i;

  for (auto n=txt.size(); i<n && !std::regex_match(txt[i], pattern); i+=1);

  std::cerr << dayfile << " skipped lines " << old_i << "-" << i << " to reach " << pattern_string << std::endl;
  
  return i;
}

vector<vector<string>> extract_paragraphs(vector<string>& txt, size_t & i, regex chapter, regex speaker, regex paragraph) {

  vector<vector<string>> paragraphs;

  for (auto n=txt.size(); i<n && !std::regex_match(txt[i], speaker) && !std::regex_match(txt[i], chapter); i+=1) {

    if (std::regex_match(txt[i], paragraph) || paragraphs.empty()) {
      paragraphs.push_back(vector<string>{ txt[i] });
    } else {
      paragraphs.back().push_back(txt[i]);
    }

  }

  return paragraphs;
}
*/
/*
void align(string preprocessor, string outdir, string dir, string l1, vector<string>& languages, string dayfile) {

  std::map< std::string, std::vector<string> > txt;
  std::map< std::string, unsigned int> size;
  std::map< std::string, unsigned int> index;
  std::map< std::string, std::ofstream > out;
  std::map< std::string, unsigned int > chapter;

  for (auto language : languages) {
    txt[language] = process(preprocessor + " -l "+ language + " < " + dir+"/"+language+"/"+dayfile);
    size[language] = txt[language].size();
    index[language] = 0;
    out[language].open(outdir+"/"+language+"/"+dayfile);
  }

  auto allIndicesValid = [&]() {
    for (auto language : languages) {
      if (index[language] >= size[language]) {
	return false;
      }
    }
    return true;
  };

  auto advanceToChapter = [&](string language) {
    smatches matches;

    for (bool found = false; !found && index[language] < size[language]; ) {

      if (std::regex_match(txt[language][index[language]], matches, chapter)) {
	chapter[language] = std::stoi(matches[1]);
	return true;
      } else {
	index[language] = skip(dayfile, txt[language], index[language], chapter, chapter_pattern);
      }

    }

    return false;

  }

  auto findMaximumChapter = [&]() {

    auto max = chapter[languages[0]];

    for (auto language : languages) {
      if (chapter[language] > max) {
	max = chapter[language];
      }
    }

    return max;
  }


  auto matchChapters = [&]() {
    chapter.clear();

    for (auto language : languages) {
      auto success = advanceToChapter(language);
      if (!success) return false;
    }
    
    auto max = findMaximumChapter();

    for (auto language : languages) {
      while (chapter[language] < max) {
	auto success = advanceToChapter();
	if (!success) return false;
      }
    }

    for (auto language : languages) {
      out[language] << txt[language][index[language]] << std::endl;
      index[language] += 1;
    }
    
    return allIndicesValid();
  }


  string chapter_pattern = R"(^<CHAPTER ID=\"?(\d+)\"?.*)";
  regex chapter{chapter_pattern};
  
  string speaker_pattern = R"(^<SPEAKER ID=\"?(\d+)\"?.*)";
  regex speaker{speaker_pattern};
  
  string paragraph_pattern = R"(^<P>.*$)";
  regex paragraph{paragraph_pattern};
  while (allIndicesValid()) {

    auto success = matchChapters();
    if (!success) return;

    
  }

}
*/
/*

void align(string preprocessor, string outdir, string dir, string l1, string l2, string dayfile) {

  auto txt1 = process(preprocessor + " -l " + l1 + " < " + dir+"/"+l1+"/"+dayfile);
  auto txt2 = process(preprocessor + " -l " + l2 + " < " + dir+"/"+l2+"/"+dayfile);

  ofstream out1;
  ofstream out2;

  out1.open(outdir+"/"+l1+"-"+l2+"/"+l1+"/"+dayfile);
  out2.open(outdir+"/"+l1+"-"+l2+"/"+l2+"/"+dayfile);

  for (size_t size1=txt1.size(), size2=txt2.size(), i1=0, i2=0; i1<size1 && i2<size2; ) {
    
    string chapter_pattern = R"(^<CHAPTER ID=\"?(\d+)\"?.*)";
    regex chapter{chapter_pattern};

    string speaker_pattern = R"(^<SPEAKER ID=\"?(\d+)\"?.*)";
    regex speaker{speaker_pattern};

    string paragraph_pattern = R"(^<P>.*$)";
    regex paragraph{paragraph_pattern};

    int s1;

    smatch matches;

    // match chapter start
    if (std::regex_match(txt1[i1], matches, chapter)) {
      auto c1 = std::stoi(matches[1]);
      if (std::regex_match(txt2[i2], matches, chapter)) { 
	auto c2 = std::stoi(matches[1]);
	if (c1 == c2) {
	  out1 << txt1[i1++] << std::endl;
	  out2 << txt2[i2++] << std::endl;
	} else if (c1 < c2) {
	  i1 = skip(dayfile, txt1, i1+1, chapter, chapter_pattern);
	} else {
	  i2 = skip(dayfile, txt2, i2+1, chapter, chapter_pattern);
	}
      } else {
	i2 = skip(dayfile, txt2, i2, chapter, chapter_pattern);
      }
    }

    else if (std::regex_match(txt1[i1], matches, speaker)) {
      s1 = std::stoi(matches[1]);
      if (std::regex_match(txt2[i2], matches, speaker)) { 
	auto s2 = std::stoi(matches[1]);
	if (s1 == s2) {
	  out1 << txt1[i1++] << std::endl;
	  out2 << txt2[i2++] << std::endl;
	} else if (s1 < s2) {
	  i1 = skip(dayfile, txt1, i1+1, speaker, speaker_pattern);
	} else {
	  i2 = skip(dayfile, txt2, i2+1, speaker, speaker_pattern);
	}
      } else {
	i2 = skip(dayfile, txt2, i2, speaker, speaker_pattern);
      }
    }

    else {
      auto p1 = extract_paragraphs(txt1, i1, chapter, speaker, paragraph);
      auto p2 = extract_paragraphs(txt2, i2, chapter, speaker, paragraph);

      if (p1.size() != p2.size()) {
	std::cerr << dayfile << " (speaker " << s1 << ") different number of paragraphs " << p1.size() << " != " << p2.size() << std::endl;
      } else {
	for (unsigned long n=p1.size(), p=0; p<n; p+=1) {
	  	  std::cerr << "Aligning paragraphs " << p << " of " << n << std::endl;
	  //sentence_align(p1[p], p2[p]);
	  //std::cerr << "Aligned paragraphs " << p << " of " << n << std::endl;
	}
      }
    }

  }


  out1.close();
  out2.close();

}
*/
/*
int main2 () {
  

  std::vector< std::vector<unsigned int> > lengths;
  //  lengths.push_back(std::vector<unsigned int>{0,5,5,3,7});
  //  lengths.push_back(std::vector<unsigned int>{0,6,5,5});
  //  lengths.push_back(std::vector<unsigned int>{0,6,4,2,6});
  lengths.push_back(std::vector<unsigned int>{30});
  lengths.push_back(std::vector<unsigned int>{24});

  Costs costs(lengths);
  Coordinates coordinates(costs);

  for (Coordinate x : coordinates) {
    for (Coordinate y : x.possiblePredecessors()) {
      costs.calculate(x, y);
    }
  }
  std::cout << std::endl;

  costs.backtrace();

  return 0;
}

*/
int main (int argc, char *argv[]) {

  string preprocessor = (argc >= 2) ? string(argv[1]) : string("tools/split-sentences.perl");
  string dir          = (argc >= 3) ? string(argv[2]) : string("txt");
  string outdir       = (argc >= 4) ? string(argv[3]) : string("aligned_cxx");

  vector<string> languages =  process("ls " + dir);

  if (languages.size() < 2) {
    std::cerr << "Can't align because " << dir << " contains fewer than 2 languages" << std::endl;
    return EXIT_FAILURE;
  }

  for (auto language : languages) {
    ensureDirectoryExists(dir + "/" + language);
    createDirectory(outdir + "/" + language);
  }

  for (auto dayfile : process("ls " + dir + "/" + languages[0])) {

    std::vector<string> present;
    std::vector<string> missing;

    for (auto language : languages) {
      if (fileExists(dir + "/" + language + "/" + dayfile)) {
	present.push_back(language);
      } else {
	missing.push_back(language);
      }
    }

    if (present.size() == languages.size()) {
      std::cerr << "Will align " << dayfile << std::endl;
      Europarl europarl(dayfile, languages, dir, outdir, preprocessor);
      europarl.align();
    } else {
      std::cerr << dayfile << " only for ";
      for (auto language : present) { std::cerr << language << " "; }
      std::cerr << ", but not for ";
      for (auto language : missing) { std::cerr << language << " "; }
      std::cerr << ", skipping" << std::endl;
    }

  }


  /*
  string l1(argv[2]);
  string l2(argv[3]);

  ensureDirectoryExists(dir + "/" + l1);
  ensureDirectoryExists(dir + "/" + l2);

  createDirectory(outdir + "/" + l1+"-"+l2 + "/" + l1);
  createDirectory(outdir + "/" + l1+"-"+l2 + "/" + l2);

  for (auto dayfile : process("ls " + dir + "/" + l1)) {

    if (fileExists(dir + "/" + l2 + "/" + dayfile) ) { 
      align(preprocessor, outdir, dir, l1, l2, dayfile);
    } else {
      std::cerr << dayfile << " only for " << l1 << ", not for " << l2 << ", skipping" << std::endl;
    }
    
  }
  */
  return EXIT_SUCCESS;
}

