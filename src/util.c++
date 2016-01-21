#include "util.h"

#include <string>
#include <iostream>
#include <sys/stat.h>


void ensureDirectoryExists(std::string dir) {
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

void createDirectory(std::string dir) {
  system(("mkdir -p " + dir).c_str());
}


