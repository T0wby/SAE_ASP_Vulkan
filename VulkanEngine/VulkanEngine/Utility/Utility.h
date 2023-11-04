#ifndef UTILITY_H
#define UTILITY_H
#include <vector>
#include <string>

class CUtility
{
public:
	static std::vector<char> ReadFile(const std::string& filename);
};

#endif