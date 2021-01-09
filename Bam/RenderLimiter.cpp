#include "common.h"

#include "RenderLimiter.h"

#include <GL/glew.h>
#include <glfw/glfw3.h>

#include "Option.h"

RenderLimiter::RenderLimiter() {
	this->lastRender = glfwGetTime();
	this->renderTime = 1.0 / static_cast<double>(Option<OPTION::GR_FRAMERATE, int32_t>::getVal());
}

void RenderLimiter::advance() {
	lastRender += this->renderTime;
	this->renderTime = 1.0 / static_cast<double>(Option<OPTION::GR_FRAMERATE, int32_t>::getVal());
}

bool RenderLimiter::ready() {
	return glfwGetTime() >= this->lastRender + this->renderTime;
}
