#include <string>
#include <vector>

void ensureDirectoryExists(std::string dir);

bool fileExists(std::string file);

void createDirectory(std::string dir);

std::vector<std::string> process(std::string command);

unsigned int utf8_codepoints(std::string& string);

unsigned int nonwhitespace_utf8_codepoints(std::string& string);

