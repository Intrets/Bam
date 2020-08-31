#include "common.h"

#include "Random.h"

#include <random>
#include <limits>

int32_t SmallRandom::randRange(int32_t min, int32_t max) {
	int32_t res = (this->values[index] % (max + 1 - min)) + min;
	index = (index + 1) % this->size;
	return res;
}

int32_t SmallRandom::rand(int32_t max) {
	return randRange(0, max);
}

SmallRandom::SmallRandom() : SmallRandom(10'007) {
}

SmallRandom::SmallRandom(int32_t size_) {
	this->size = size_;
	this->valueRangeMax = std::numeric_limits<int32_t>::max();

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(0, this->valueRangeMax);

	for (size_t i = 0; i < this->size; i++) {
		this->values.push_back(distrib(gen));
	}
}
