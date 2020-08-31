#pragma once

#include <cstdint>

class SmallRandom
{
private:
	int32_t size;
	std::vector<int32_t> values;
	int32_t index = 0;

	int32_t valueRangeMax;

public:
	// [min, max]
	int32_t randRange(int32_t min, int32_t max);

	// [0, max]
	int32_t rand(int32_t max);

	SmallRandom();
	SmallRandom(int32_t size);
	~SmallRandom() = default;
};

