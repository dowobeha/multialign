#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <regex>
#include <cmath>
#include <utility>

#include "util.h"

#include "MultextEast.h++"

int main (int argc, char *argv[]) {

  std::string dir          = (argc >= 2) ? std::string(argv[1]) : std::string("orwell");
  std::string outdir       = (argc >= 3) ? std::string(argv[2]) : std::string("orwell_gold");

  std::vector<std::string> languages =  process("ls " + dir);

  if (languages.size() < 2) {
    std::cerr << "Can't align because " << dir << " contains fewer than 2 languages" << std::endl;
    return EXIT_FAILURE;
  }

  for (auto language : languages) {
    ensureDirectoryExists(dir + "/" + language);
    createDirectory(outdir + "/" + language);
  }

  for (auto dayfile : process("ls " + dir + "/" + languages[0])) {

    std::vector<std::string> present;
    std::vector<std::string> missing;

    for (auto language : languages) {
      if (fileExists(dir + "/" + language + "/" + dayfile)) {
	present.push_back(language);
      } else {
	missing.push_back(language);
      }
    }

    if (present.size() == languages.size()) {
      std::cerr << "Will align " << dayfile << std::endl;
      MultextEast orwell(dayfile, languages, dir, outdir);
      orwell.align();
    } else {
      std::cerr << dayfile << " only for ";
      for (auto language : present) { std::cerr << language << " "; }
      std::cerr << ", but not for ";
      for (auto language : missing) { std::cerr << language << " "; }
      std::cerr << ", skipping" << std::endl;
    }

  }

  return EXIT_SUCCESS;
}

