#pragma once

#include <sstream>
#include <mutex>
#include <vector>

class Log
{
private:
	std::mutex mtx;

public:
	struct CLOSE {};
	struct OPEN {};

	std::vector<std::string> lines;

	std::ostringstream buffer;

	void putLine(std::string str);
	std::vector<std::string> getLines();

	friend Log& operator<<(Log& log, const Log::CLOSE&);
	friend Log& operator<<(Log& log, const Log::OPEN&);

	template<class T>
	friend Log& operator<<(Log& log, const T& data);

	friend Log& operator<<(Log& log, const char* d);
};

template<>
inline Log& operator<<(Log& log, const std::string& data) {
	if (data.back() == '\n') {
		log.buffer << data.substr(0, data.size() - 1);
		log.lines.push_back(log.buffer.str());
		log.buffer = std::ostringstream();
	}
	else {
		log.buffer << data;
	}
	return log;
}

inline Log& operator<<(Log& log, const Log::CLOSE&) {
	log.mtx.unlock();
	return log;
}

inline Log& operator<<(Log& log, const Log::OPEN&) {
	log.mtx.lock();
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
