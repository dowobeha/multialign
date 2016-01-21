#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <regex>

#include "util.h"

using std::string;
using std::vector;
using std::regex;
using std::ofstream;
using std::smatch;

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
      //      std::cerr << "New paragraph " << paragraphs.size() << ": "  << txt[i] << std::endl;
      paragraphs.push_back(vector<string>{ txt[i] });
    } else {
      //      std::cerr << "Add to existing paragraph " << paragraphs.size() << ": "  << txt[i] << std::endl;
      paragraphs.back().push_back(txt[i]);
    }

  }

  return paragraphs;
}

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
      //      std::cerr << "About to extract paragraph, starting at i1==" << i1 << std::endl;
      auto p1 = extract_paragraphs(txt1, i1, chapter, speaker, paragraph);
      //      std::cerr << "Extracted paragraph, now at i1==" << i1 << std::endl;
      //      std::cerr << "About to extract paragraph, starting at i2==" << i2 << std::endl;
      auto p2 = extract_paragraphs(txt2, i2, chapter, speaker, paragraph);
      //      std::cerr << "Extracted paragraph, now at i2==" << i2 << std::endl;
      if (p1.size() != p2.size()) {
	std::cerr << dayfile << " (speaker " << s1 << ") different number of paragraphs " << p1.size() << " != " << p2.size() << std::endl;
      } else {

      }
    }

  }


  out1.close();
  out2.close();

}

int main (int argc, char *argv[]) {

  string dir("txt");
  string outdir("aligned_cxx");

  if (argc < 4) {
    std::cerr << "Usage:\t" << argv[0] << " split-sentences.perl l1 l2" << std::endl;
    return EXIT_FAILURE;
  }

  string preprocessor(argv[1]);
  string l1(argv[2]);
  string l2(argv[3]);

  ensureDirectoryExists(dir + "/" + l1);
  ensureDirectoryExists(dir + "/" + l2);

  createDirectory(outdir + "/" + l1+"-"+l2 + "/" + l1);
  createDirectory(outdir + "/" + l1+"-"+l2 + "/" + l2);
  /*
  {
    redi::ipstream ls("ls " + dir + "/" + l1);
    string dayfile;
    while (ls >> dayfile) {
      
      if (fileExists(dir + "/" + l2 + "/" + dayfile) ) { 
	align(preprocessor, outdir, dir, l1, l2, dayfile);
      } else {
	std::cerr << dayfile << " only for " << l1 << ", not for " << l2 << ", skipping" << std::endl;
      }
    }

  } 
  */

  for (auto dayfile : process("ls " + dir + "/" + l1)) {

    if (fileExists(dir + "/" + l2 + "/" + dayfile) ) { 
      align(preprocessor, outdir, dir, l1, l2, dayfile);
    } else {
      std::cerr << dayfile << " only for " << l1 << ", not for " << l2 << ", skipping" << std::endl;
    }
    
  }

  return EXIT_SUCCESS;
}

int main2 (int argc, char *argv[]) {
  /*
  std::ifstream in("table.txt");

  if (!in) std::cerr << "no file\n";

  string line;
  int lineno = 0;

  std::regex header {R"(ˆ[\w ]+(\t[\w ]+)∗$)"};
  std::regex row {R"(ˆ([\w ]+)(\t\d+)(\t\d+)(\t\d+)$)"};
  */

  std::regex r1("S");
  printf("S works.\n");
  std::regex r2(".");
  printf(". works.\n");
  std::regex r3(".+");
  printf(".+ works.\n");
  std::regex r4("[0-9]");
  printf("[0-9] works.\n");
  return 0;

}
