#include "common.h"
#include "GLEnableWrapper.h"

GLEnabler& GLEnabler::enable(int h) {
	enabled.push_back(h);
	glEnable(h);
	return *this;
}

GLEnabler& GLEnabler::disable(int h) {
	disabled.push_back(h);
	glDisable(h);
	return *this;
}

GLEnabler::~GLEnabler() {
	for (int h : enabled) {
		glDisable(h);
	}
	for (int h : disabled) {
		glEnable(h);
	}
}
