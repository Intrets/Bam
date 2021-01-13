#include "common.h"

#include "Uuid.h"

#include <ctime>

std::string UUID::getUUID() {
	std::time_t now = std::time(nullptr);
	return std::to_string(now);
}
