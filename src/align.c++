#include <iostream>
#include <cstdlib>

#include "util.h"


int main (int argc, char *argv[]) {

  std::string dir("txt");
  std::string outdir("aligned_cxx");
  std::string preprocessor("/projects/cl/ducttape_for_moses/tools/split-sentences.perl");

  if (argc < 3) {
    std::cerr << "Usage:\t" << argv[0] << " l1 l2" << std::endl;
    return EXIT_FAILURE;
  }

  std::string l1(argv[1]);
  std::string l2(argv[2]);

  ensureDirectoryExists(dir + "/" + l1);
  ensureDirectoryExists(dir + "/" + l2);

  createDirectory(outdir + "/" + l1+"-"+l2 + "/" + l1);
  createDirectory(outdir + "/" + l1+"-"+l2 + "/" + l2);



  return EXIT_SUCCESS;
}
