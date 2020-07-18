#include "common.h"

#include "Option.h"
#include <sstream>
#include <fstream>

#include "StringHelpers.h"


void OptionManager::defaultValues() {
	initVal(OPTIONS2::CL_VIEWPORTSCALE, 20.0f, "cl_viewportscale", "");
	initVal(OPTIONS2::GR_DEBUG, false, "gr_debug", "");
	initVal(OPTIONS2::GR_FRAMESYNCMODE, 0, "gr_framesyncmode", "");
}

void OptionManager::readFromFile() {
	std::ifstream file;

	if (Locator<PathManager>::get()->openFile(file, RESOURCE_FILE::OPTIONS)) {
		std::string line;
		while (std::getline(file, line)) {
			std::vector<std::string> parts;
			split(0, line, parts);

			if (parts.size() < 3) {
				continue;
			}

			std::string name = parts[0];
			auto f = nameMap.find(name);

			if (f == nameMap.end()) {
				continue;
			}

			OPTIONS2 optionIndex = f->second;

			data[optionIndex]->set(parts[2]);
		}
		file.close();
	}
}

void OptionManager::writeToFile() {
	std::ofstream file;
	if (Locator<PathManager>::get()->openFile(file, RESOURCE_FILE::OPTIONS)) {
		for (auto& obj : data) {
			file << obj->toString() << "\n";
		}
		file.close();
	}
}

OptionManager::OptionManager() {
	defaultValues();
	readFromFile();
}

OptionManager::~OptionManager() {
}

std::string _OptionValueBase::getName() {
	return name;
}

std::string _OptionValueBase::getDescription() {
	return description;
}

std::string _OptionValueBase::toString() {
	std::stringstream out;
	toStream(out);
	return out.str();
}
