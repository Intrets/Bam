#include "common.h"

#include "Enums.h"

ELEMENT::TYPE ELEMENT::getType(std::string s) {
	return ELEMENT::typeMap[s];
}

std::string ELEMENT::getName(ELEMENT::TYPE type) {
	switch (type) {
		case ELEMENT::AIR:
			return "air";
			break;
		case ELEMENT::SILICONDIOXIDE:
			return "silicondioxide";
			break;
		case ELEMENT::IRON:
			return "iron";
			break;
		case ELEMENT::IRONOXIDE:
			return "ironoxide";
			break;
		case ELEMENT::CARBON:
			return "carbon";
			break;
		case ELEMENT::HYDROCARBON:
			return "hydrocarbon";
			break;
		default:
			return "";
			break;
	}
}

