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

using std::string;
using std::vector;
using std::regex;
using std::ofstream;
using std::smatch;
using std::pair;

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

double pnorm(double z) {
  auto t = 1/(1 + 0.2316419 * z);
  return 1 - 0.3989423 * exp(-z * z / 2) *
    ((((1.330274429 * t 
	- 1.821255978) * t 
       + 1.781477937) * t 
      - 0.356563782) * t
     + 0.319381530) * t;
}

double match(unsigned int len1, unsigned int len2) {

  double c = 1;
  double s2 = 6.8;

  if (len1==0 && len2==0) { return 0; }
  double mean = (len1 + len2/c) / 2;
  double z = (c * len1 - len2)/sqrt(s2 * mean);
  if (z < 0) { z = -z; }
  double pd = 2 * (1 - pnorm(z));
  if (pd>0) { return -log(pd); }
  return 25;
}

double prior(unsigned int a, unsigned int b) {
  if (a==1 && b==1) return 0.89;
  else if (a==1 && b==0) return 0.01/2;
  else if (a==0 && b==1) return 0.01/2;
  else if (a==2 && b==1) return 0.089/2;
  else if (a==1 && b==2) return 0.089/2;
  else return 0.0;
}

vector<unsigned int> prior_keys(unsigned int a) { 
  if (a==0) return vector<unsigned int>{ 1 };
  else if (a==1) return vector<unsigned int>{ 0, 1, 2 };
  else if (a==2) return vector<unsigned int>{ 1 };
  else return vector<unsigned int> {};
}

template <class T>
class Values {

private:

  unsigned int ny;
  unsigned int nx;
  vector<T> values;

public:

  Values(unsigned int n1, unsigned int n2) 
    : ny(n2), nx(n1), values((n1+1)*(n2+1), 0.0) 
  { 
    
    //    std::cerr << "values:\t" << values.size() << std::endl;

}

  T get(unsigned int x, unsigned int y) {

    return values[x * (ny+1) + y];
    ///return 0.0;
  }

  void set(unsigned int x, unsigned int y, T value) {
    //    std::cerr << "Values.set(" << x << "," << y << "," << value << ") where values.size()==" << values.size() << "(nx==" << nx << ", ny==" << ny << ") and index into values is " << (x * (ny+1) + y) << std::endl;
    values[x * (ny+1) + y] = value;
  }

};
/*
class Back {

private:
  unsigned int ny;
  vector<int> path;

public:
  
  Back(unsigned int n1, unsigned int n2): ny(n2), path((n1+1)*(n2+1), 0) {}

  int get(unsigned int x, unsigned int y) {
    return path[x * (ny+1) + y];
  }

  void set(unsigned int x, unsigned int y, int value) {
    path[x * (ny+1) + y] = value;
  }

}
*/
void sentence_align(const vector<string>& p1, const vector<string>& p2) {



  vector<unsigned int> keys{ 0, 1, 2};

  vector<unsigned int> len1(p1.size()+1, 0);
  vector<unsigned int> len2(p2.size()+1, 0);
  
  for (unsigned int n=p1.size(), i=0; i<n; i+=1) {
    //    std::cerr << "p1[" << i<< "]:\t" << p1[i] << std::endl;
    auto line = p1[i];
    len1[i+1] = len1[i] + nonwhitespace_utf8_codepoints(line);
  }
  
  for (unsigned int n=p2.size(), i=0; i<n; i+=1) {
    //std::cerr << "p2[" << i<< "]:\t" << p2[i] << std::endl;
    auto line = p2[i];
    len2[i+1] = len2[i] + nonwhitespace_utf8_codepoints(line);
  }


  Values<int> pathX(p1.size(), p2.size());
  Values<int> pathY(p1.size(), p2.size());

  // dynamic programming
  {

    //  std::cerr << "Starting dynamic programming inside sentence_align" << std::endl;
    Values<double> cost(p1.size(), p2.size());
  //  vector<double> inner_cost(p2.size()+1, 0.0);
  //  vector<vector<double> > cost(p1.size()+1, vector<double>(p2.size()+1, 0.0) );

  //  vector<pair<int,int> > inner_back(p2.size()+1, std::pair<int,int>(0,0));
  //  vector<vector<pair<int,int> > > back(p1.size()+1, vector<pair<int,int> >(p2.size()+1, std::pair<int,int>(0,0)));

  for (unsigned int n1=p1.size(), i1=0; i1<n1; i1+=1) {   //std::cout << "here1" << std::endl;
    for (unsigned int n2=p1.size(), i2=0; i2<n2; i2+=1) {   //std::cout << "here2" <<std::endl;
	if (i1+i2==0) continue;
	//std::cout << "i1==" << i1 << " i2==" << i2 << " p1.size()==" << p1.size() << " p2.size()==" << p2.size() <<std::endl;
	cost.set(i1,i2,1e10);   
	//std::cout << "here3 " << "i1==" << i1 << " i2==" << i2 << " p1.size()==" << p1.size() << " p2.size()==" << p2.size() <<std::endl;
	for (auto d1 : keys) {   //std::cout << "here4" <<std::endl;
	  if (d1 > i1) continue;
	  for (auto d2 : prior_keys(d1)) {   //std::cout << "here5" <<std::endl;
	    if (d2 > i2) continue;   //std::cout << "here6" <<std::endl;
	    auto new_cost = cost.get(i1-d1, i2-d2) - log(prior(d1,d2)) + 
	      match(len1[i1]-len1[i1-d1], len2[i2]-len2[i2-d2]);
	    // std::cout << "here7 " << "i1==" << i1 << " i2==" << i2 << " p1.size()==" << p1.size() << " p2.size()==" << p2.size() <<std::endl;
	    if (new_cost < cost.get(i1,i2)) {
	      // std::cout << "here8" <<std::endl;
	      cost.set(i1,i2,new_cost);
	      // std::cout << "here9" <<std::endl;
	      //back[i1][i2] = std::make_pair(i1-d1, i2-d2);
	      pathX.set(i1,i2,i1-d1);
	      pathY.set(i1,i2,i2-d2);
	    }
	  }
	}
      }
  }
  //  std::cerr << "Done with dynamic programming inside sentence_align\tp1.size()==" << p1.size() << " p2.size()==" << p2.size() << std::endl;
  }
  //  std::cerr << "Done with sentence_align" << std::endl;

  /*  
  for (auto s1 : p1) {
    std::cerr << "utf codepoint count in \"" << s1 << "\" == " << utf8_codepoints(s1) << " vs s1.size()==" << s1.size() << " and nonwhitespace==" << nonwhitespace_utf8_codepoints(s1) << std::endl;
  }
  */

  // back tracking
  {    
    Values<int> nextX(p1.size(), p2.size());
    Values<int> nextY(p1.size(), p2.size());
    
    auto i1 = p1.size();
    auto i2 = p2.size();

    while (i1>0 || i2>0) {
      nextX.set(pathX.get(i1,i2), pathY.get(i1,i2), i1);
      nextY.set(pathX.get(i1,i2), pathY.get(i1,i2), i2);

      auto new_i1 = pathX.get(i1,i2);
      auto new_i2 = pathY.get(i1,i2);

      i1 = new_i1;
      i2 = new_i2;
    }
    
    while (i1<p1.size() || i2<p2.size()) {

      std::cout << "P1:\t";
      for (auto i=i1; i<nextX.get(i1,i2); i++) {
	if (i != i1) std::cout << " ";
	std::cout << p1[i];
      }
      std::cout << std::endl;

      std::cout << "P2:\t";
      for (auto i=i2; i<nextY.get(i1,i2); i++) {
	std::cerr << "i==" << i << " p2.size()==" << p2.size() << std::endl;
	if (i != i2) std::cout << " ";
	std::cout << p2[i];
      }
      std::cout << std::endl;

      auto new_i1 = nextX.get(i1,i2);
      auto new_i2 = nextY.get(i1,i2);

      i1 = new_i1;
      i2 = new_i2;

    }

  }


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
      auto p1 = extract_paragraphs(txt1, i1, chapter, speaker, paragraph);
      auto p2 = extract_paragraphs(txt2, i2, chapter, speaker, paragraph);

      if (p1.size() != p2.size()) {
	std::cerr << dayfile << " (speaker " << s1 << ") different number of paragraphs " << p1.size() << " != " << p2.size() << std::endl;
      } else {
	for (unsigned long n=p1.size(), p=0; p<n; p+=1) {
	  //	  std::cerr << "Aligning paragraphs " << p << " of " << n << std::endl;
	  sentence_align(p1[p], p2[p]);
	  //std::cerr << "Aligned paragraphs " << p << " of " << n << std::endl;
	}
      }
    }

  }


  out1.close();
  out2.close();

}


int main () {
  
  //  int numbers[5]={10,20,30,40,50};
  
  //  int *n = &numbers[0];

  std::vector< std::vector<unsigned int> > lengths;
  lengths.push_back(std::vector<unsigned int>{0,10});
  lengths.push_back(std::vector<unsigned int>{0,6,4});

  Costs costs(lengths);

  /*
MyIterator from(numbers);
  MyIterator until(numbers+5);
  for (MyIterator it=from; it!=until; it++)
    std::cout << *it << ' ';
  std::cout << '\n';
  */

  Coordinates coordinates{1,2};

  for (Coordinate x : coordinates) {
    //    std::cout << x << std::endl;

    //    auto predecessors = x.possiblePredecessors();

    //    for (Coordinate y : predecessors) {
    for (Coordinate y : coordinates) {
      //      std::cout << "\t" << y << std::endl;
      costs.calculate(x, y);
    }
  }
  std::cout << std::endl;

  return 0;
}
/*

int main4( int argc, char *argv[]) {

  //Coordinate zero(vector<unsigned int>{4, 3});
  Coordinate zero {4, 3};

  Coordinate final = zero.final();

  if (zero < final) {
    std::cout << (zero<final) << " " << (zero==final) << std::endl;
  } else {
    std::cout << "bad" << std::endl;
  }


  

}

int main3 (int argc, char *argv[]) {

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
*/
