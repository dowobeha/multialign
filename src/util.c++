#include "util.h"

#include <string>
#include <iostream>
#include <vector>
#include <sys/stat.h>

#include "pstream.h"

#include "utf8.h"

using std::string;
using std::vector;

void ensureDirectoryExists(string dir) {
  struct stat buffer;

  bool exists = (stat(dir.c_str(), &buffer) == 0);
  bool isDir  = S_ISDIR(buffer.st_mode);

  if (exists && isDir) {
    return;
  } else {
    std::cerr << "Expected directory " << dir << " not found" << std::endl;
    exit(EXIT_FAILURE);
  }
}

bool fileExists(string file) {
  struct stat buffer;

  bool exists = (stat(file.c_str(), &buffer) == 0);

  if (exists) {
    return true;
  } else {
    return false;
  }
}

void createDirectory(string dir) {
  system(("mkdir -p " + dir).c_str());
}

vector<string> process(string command) {
  vector<string> results; {
    redi::ipstream stream(command);
    for (string line; std::getline(stream, line); ) { 
     results.push_back(line);
    }
  }
  return results;
}


/*
vector<string> preprocess(string preprocessor, string dir, string lang, string dayfile) {
  vector<string> txt; {
    redi::ipstream stream(preprocessor + " -l " + l1 + " < " + dir+"/"+l1+"/"+dayfile);
    for (string line; std::getline(stream, line); ) {
      txt.push_back(line);
    }
  }
  return txt;
}
*/


unsigned int utf8_codepoints(string& string) {

  auto begin = string.begin();
  auto end = string.end();
  auto d = utf8::distance(begin, end);

  return d;
}

unsigned int nonwhitespace_utf8_codepoints(string& string) {

  unsigned int count = 0;

  auto begin = string.begin();
  auto end = string.end();

  for (auto current=begin; current != end ; ) {
   
    auto previous = current;
    utf8::next(current,end);
    
    if (current - previous > 1) {
      count += 1;
    } else {
      auto c = *current;
      if (c != ' '  &&
	  c != '\r' &&
	  c != '\n' &&
	  c != '\t') {
	count += 1;
      }
    }
    
  }

  return count;
}
