#pragma once
#include <string>
#include <vector>
#include <istream>
struct Task{
	std::string dmslCode;
	std::string output;
};

std::vector<Task> ParseMakefile(std::istream& make);