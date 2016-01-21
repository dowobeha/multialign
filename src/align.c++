#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <regex>

#include "util.h"
#include "pstream.h"

void align(std::string preprocessor, std::string outdir, std::string dir, std::string l1, std::string l2, std::string dayfile) {

  std::vector<std::string> txt1; {
    redi::ipstream txt1native(preprocessor + " -l " + l1 + " < " + dir+"/"+l1+"/"+dayfile);
    for (std::string line; std::getline(txt1native, line); ) {
      txt1.push_back(line);
    }
  }

  std::vector<std::string> txt2; {
    redi::ipstream txt2native(preprocessor + " -l " + l2 + " < " + dir+"/"+l2+"/"+dayfile);
    for (std::string line; std::getline(txt2native, line); ) {
      txt2.push_back(line);
    }
  }

  std::ofstream out1;
  std::ofstream out2;

  out1.open(outdir+"/"+l1+"-"+l2+"/"+l1+"/"+dayfile);
  out2.open(outdir+"/"+l1+"-"+l2+"/"+l2+"/"+dayfile);

  for (size_t size1=txt1.size(), size2=txt2.size(), i1=0, i2=0; i1<size1 && i2<size2; ) {
    
    std::regex pattern{R"(<CHAPTER.*)"};
    //    std::regex pattern{R"(^<CHAPTER.*)"};
    //    std::regex pattern { R"(ˆA∗B+C?$)" };
    //    std::regex pattern { R"(.*)"};
    //std::regex pattern {R"(ˆ[\w ]+(\t[\w ]+)∗$)"};
    std::smatch matches;



    if (std::regex_match(txt1[i1], pattern, std::regex_constants::match_continuous)) {
      std::cout << txt1[i1] << std::endl;
    }


    i1 += 1U;
    i2 += 1U;
  }


  out1.close();
  out2.close();

}

int main2 (int argc, char *argv[]) {

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
	align(preprocessor, outdir, dir, l1, l2, dayfile);
      } else {
	std::cerr << dayfile << " only for " << l1 << ", not for " << l2 << ", skipping" << std::endl;
      }
    }

  } 


  return EXIT_SUCCESS;
}

int main (int argc, char *argv[]) {
  /*
  std::ifstream in("table.txt");

  if (!in) std::cerr << "no file\n";

  std::string line;
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
