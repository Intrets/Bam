#include "common.h"

#include "Timer.h"

#include <sstream>
#include <iomanip>

void Timer::startTiming(std::string timingName) {
	std::lock_guard<std::mutex> guard(this->mtx);

	auto it = this->timings.find(timingName);
	if (it == this->timings.end()) {
		return;
	}
	it->second.maybeStart = glfwGetTime();
}

void Timer::endTiming(std::string timingName) {
	std::lock_guard<std::mutex> guard(this->mtx);

	auto it = this->timings.find(timingName);
	if (it == this->timings.end()) {
		return;
	}

	auto& timing = it->second;
	if (timing.maybeStart.has_value()) {
		timing.timing += glfwGetTime() - timing.maybeStart.value();
		timing.maybeStart = std::nullopt;
	}
}

void Timer::newTiming(std::string timingName) {
	std::lock_guard<std::mutex> guard(this->mtx);

	auto [it, b] = this->timings.insert({ timingName, Timing() });
	auto& timing = it->second;
	if (!b) {
		timing.history.insert(timing.timing);
		timing.timing = 0.0;
	}
	timing.maybeStart = glfwGetTime();
}

std::vector<std::string> Timer::print() {
	std::vector<std::string> res;
	std::stringstream out;
	out << std::setprecision(3);

	size_t pad = 0;
	for (auto& p : this->timings) {
		pad = glm::max(pad, p.first.size());
	}

	for (auto& p : this->timings) {
		double average = p.second.history.average(0.0);
		out << std::setw(pad) << p.first << ": " << std::setw(7) << average * 1000.0 << "ms | " << 1.0 / average;
		res.push_back(out.str());
		out.str(std::string());
	}

	return res;
}
