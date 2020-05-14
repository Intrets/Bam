#pragma once

#include "common.h"

class GLEnabler
{
private:
	std::vector<int32_t> enabled;
	std::vector<int32_t> disabled;
public:
	GLEnabler& enable(int32_t h);
	GLEnabler& disable(int32_t h);

	GLEnabler() = default;
	~GLEnabler();

	deleteDefaults(GLEnabler);
};