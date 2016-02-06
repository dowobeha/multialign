#pragma once

#include <iostream>
#include <fstream>
#include <functional>
#include <map>
#include <memory>
#include <regex>
#include <string>
#include <vector>

#include "Costs.h++"
#include "Coordinate.h++"
#include "Coordinates.h++"

class EuroparlFileDeleter {

public:

  void operator()(std::ofstream *o) const {
    o->close();
    delete o;
  }

};

class Europarl {

private:

  const std::string dayfile;
  const std::string dir;
  const std::string outdir;
  const std::string preprocessor;
  const std::vector<std::string> languages;

  std::map< std::string, std::vector<std::string> > txt;
  std::map< std::string, unsigned int> size;
  std::map< std::string, unsigned int> index;
  //  std::map< std::string, std::unique_ptr<std::ofstream, EuroparlFileDeleter> > out;
  std::map< std::string, std::unique_ptr<std::ofstream, std::function<void(std::ofstream *)> > > out;
  std::map< std::string, unsigned int > chapter;
  std::map< std::string, unsigned int > speaker;
  std::map< std::string, std::vector< std::vector<std::string> > > paragraphs;

  static const std::string chapter_pattern;
  static const std::regex chapter_regex;
  
  static const std::string speaker_pattern;
  static const std::regex speaker_regex;
  
  static const std::string paragraph_pattern;
  static const std::regex paragraph_regex;


  bool skip(std::string language, std::regex pattern, std::string pattern_string);
  
  bool allIndicesValid();

  bool advanceIfNeeded(std::string language, std::map< std::string, unsigned int>& map, std::string pattern, std::regex regex);

  bool match(std::map< std::string, unsigned int>& map, std::string pattern, std::regex regex);

  bool anyMatch(std::regex regex);

  bool extractParagraphs();

  

  unsigned int findMaximum(std::map< std::string, unsigned int>& map);


public:

  Europarl(std::string dayfile, std::vector<std::string> languages, std::string dir, std::string outdir, std::string preprocessor);

  void align();

};
