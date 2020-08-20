#include "common.h"

#include "TickLimiter.h"

bool TickLimiter::ready() {
	return glfwGetTime() >= tickTime + lastTick;
}

void TickLimiter::advance() {
	lastTick += tickTime;
}
