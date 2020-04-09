#pragma once

#include "common.h"

class GLEnabler
{
private:
	std::vector<int> enabled;
	std::vector<int> disabled;
public:
	GLEnabler& enable(int h);
	GLEnabler& disable(int h);

	GLEnabler() = default;
	~GLEnabler();

	deleteDefaults(GLEnabler);
};