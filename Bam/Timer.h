#pragma once

#include "CircularArray.h"
#include <mutex>
#include <optional>

struct Timing
{
	CircularArray<double> history;
	double timing;
	std::optional<double> maybeStart;

	Timing() : history(0.0f, 200), timing(0.0), maybeStart(std::nullopt) {};
};


class Timer
{
public:
	std::mutex mtx;

	std::unordered_map<std::string, Timing> timings;

	void startTiming(std::string timing);
	void endTiming(std::string timing);
	void newTiming(std::string timing);

	std::vector<std::string> print();
};

