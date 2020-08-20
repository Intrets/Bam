#include "common.h"

#include "TickLimiter.h"

#include "Option.h"

TickLimiter::TickLimiter() {
	this->lastTick = glfwGetTime();
	this->tickTime = 1.0 / static_cast<double>(Option<OPTION::LO_TICKSPERSECOND, int32_t>::getVal());
}

void TickLimiter::advance() {
	lastTick += this->tickTime;
	this->tickTime = 1.0 / static_cast<double>(Option<OPTION::LO_TICKSPERSECOND, int32_t>::getVal());
}

bool TickLimiter::ready() {
	return glfwGetTime() >= this->lastTick + this->tickTime;
}
