#include <string>
#include <vector>

void ensureDirectoryExists(std::string dir);

bool fileExists(std::string file);

void createDirectory(std::string dir);

std::vector<std::string> process(std::string command);
