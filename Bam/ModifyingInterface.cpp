#include "common.h"

#include "ModifyingInterface.h"

#include <sstream>

std::string ModifyingInterface::getMembers() {
	std::stringstream out;
	getMembers(out);
	return out.str();
}

std::string ModifyingInterface::getSimpleInfo() {
	std::stringstream out;
	getSimpleInfo(out);
	return out.str();
}
