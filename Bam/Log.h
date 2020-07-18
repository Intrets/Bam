#pragma once

#include <sstream>
#include <vector>

class Log
{
public:
	std::vector<std::string> lines;

	std::ostringstream buffer;
	
	void flush();

	template<class T>
	friend Log& operator<<(Log& log, const T& data);

	friend Log& operator<<(Log& log, const char* d);
};

template<>
inline Log& operator<<(Log& log, const std::string& data) {
	if (data.back() == '\n') {
		log.buffer << data.substr(0, data.size() - 1);
		log.flush();
	}
	else {
		log.buffer << data;
	}
	return log;
}

inline Log& operator<<(Log& log, const char* d) {
	std::string data(d);
	log << data;
	return log;
}

template<class T>
inline Log& operator<<(Log& log, const T& data) {
	log.buffer << data;
	return log;
}
