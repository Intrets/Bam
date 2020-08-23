#pragma once

#include <sstream>
#include <mutex>
#include <vector>
#include <sstream>

class Log
{
private:
	std::mutex mtx;

	std::vector<std::string> lines;

public:
	void putLine(std::string str);

	// returns a copy of the lines and clears the lines 
	std::vector<std::string> getLines();

	void putStreamLine(std::stringstream ss);
};
