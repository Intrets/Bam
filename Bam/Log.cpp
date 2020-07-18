#include "common.h"

#include "Log.h"

//void Log::append(std::string message) {
//	lines.push_back(message);
//}

void Log::flush() {
	this->lines.push_back(this->buffer.str());
	this->buffer = std::ostringstream();
}
