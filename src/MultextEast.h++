#pragma once

#include <iostream>
#include <fstream>
#include <functional>
#include <map>
#include <memory>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

class MultextEastID {

private:

  std::vector<unsigned int> value;

  public:

  MultextEastID() = default;

  MultextEastID(std::string id);

  bool operator <(const MultextEastID& that) const;

  friend std::ostream& operator<<(std::ostream& os, const MultextEastID& id);
  
};



class MultextEastFileDeleter {

public:

  void operator()(std::ofstream *o) const {
    o->close();
    delete o;
  }

};

class MultextEast {

private:

  const std::string dayfile;
  const std::string dir;
  const std::string outdir;
  const std::string preprocessor;
  const std::vector<std::string> languages;

  std::map< std::string, std::vector<std::string> > txt;
  std::map< std::string, unsigned int> size;
  std::map< std::string, unsigned int> index;
  //  std::map< std::string, std::unique_ptr<std::ofstream, MultextEastFileDeleter> > out;
  std::map< std::string, std::unique_ptr<std::ofstream, std::function<void(std::ofstream *)> > > out;
  std::map< std::string, MultextEastID > paragraph;
  std::map< std::string, std::vector<std::string> > sentences;

  static const std::string chapter_pattern;
  static const std::regex chapter_regex;
  
  static const std::string paragraph_pattern;
  static const std::regex paragraph_regex;
  
  static const std::string sentence_pattern;
  static const std::regex sentence_regex;


  bool skip(std::string language, std::map< std::string, MultextEastID>& map, std::regex pattern, std::string pattern_string);
  
  bool allIndicesValid();

  bool advanceIfNeeded(std::string language, std::map<std::string, MultextEastID>& map, std::string pattern, std::regex regex);

  bool match(std::map< std::string, MultextEastID>& map, std::string pattern, std::regex regex);

  bool anyMatch(std::regex regex);

  bool extractSentences();

  

  MultextEastID findMaximum(std::map< std::string, MultextEastID>& map);


public:

  MultextEast(std::string dayfile, std::vector<std::string> languages, std::string dir, std::string outdir);

  void align();

};
