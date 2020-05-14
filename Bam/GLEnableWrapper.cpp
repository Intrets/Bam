#include "common.h"
#include "GLEnableWrapper.h"

GLEnabler& GLEnabler::enable(int32_t h) {
	enabled.push_back(h);
	glEnable(h);
	return *this;
}

GLEnabler& GLEnabler::disable(int32_t h) {
	disabled.push_back(h);
	glDisable(h);
	return *this;
}

GLEnabler::~GLEnabler() {
	for (int32_t h : enabled) {
		glDisable(h);
	}
	for (int32_t h : disabled) {
		glEnable(h);
	}
}
