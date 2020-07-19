#include "common.h"

#include "Log.h"

void Log::putLine(std::string str) {
	std::lock_guard<std::mutex> lock(this->mtx);
	lines.push_back(str);
}

std::vector<std::string> Log::getLines() {
	std::lock_guard<std::mutex> lock(this->mtx);
	auto res = this->lines;
	this->lines.clear();
	return res;
}