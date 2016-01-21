#include <iostream>
#include <cstdlib>

#include "util.h"
#include "pstream.h"

void align(std::string preprocessor, std::string dir, std::string l1, std::string l2, std::string dayfile) {

  redi::ipstream txt1native(preprocessor + " -l " + l1 + " < " + dir+"/"+l1+"/"+dayfile);
  for (std::string line; std::getline(txt1native, line); ) {
    std::cout << line << std::endl;
  }

}

int main (int argc, char *argv[]) {

  std::string dir("txt");
  std::string outdir("aligned_cxx");

  if (argc < 4) {
    std::cerr << "Usage:\t" << argv[0] << " split-sentences.perl l1 l2" << std::endl;
    return EXIT_FAILURE;
  }

  std::string preprocessor(argv[1]);
  std::string l1(argv[2]);
  std::string l2(argv[3]);

  ensureDirectoryExists(dir + "/" + l1);
  ensureDirectoryExists(dir + "/" + l2);

  createDirectory(outdir + "/" + l1+"-"+l2 + "/" + l1);
  createDirectory(outdir + "/" + l1+"-"+l2 + "/" + l2);

  {
    redi::ipstream ls("ls " + dir + "/" + l1);
    std::string dayfile;
    while (ls >> dayfile) {
      
      if (fileExists(dir + "/" + l2 + "/" + dayfile) ) { 
	align(preprocessor, dir, l1, l2, dayfile);
      } else {
	std::cerr << dayfile << " only for " << l1 << ", not for " << l2 << ", skipping" << std::endl;
      }
    }

  } 


  return EXIT_SUCCESS;
}
