#include "common.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GLEnableWrapper.h"

GLEnabler& GLEnabler::enable(int32_t h) {
	this->enabled.push_back(h);
	glEnable(h);
	return *this;
}

GLEnabler& GLEnabler::disable(int32_t h) {
	this->disabled.push_back(h);
	glDisable(h);
	return *this;
}

GLEnabler::~GLEnabler() {
	for (int32_t h : this->enabled) {
		glDisable(h);
	}
	for (int32_t h : this->disabled) {
		glEnable(h);
	}
}
