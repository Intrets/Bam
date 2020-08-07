#include "common.h"

#include "Option.h"
#include <sstream>
#include <fstream>

#include "StringHelpers.h"


void OptionManager::defaultValues() {
	initVal(OPTION::CL_VIEWPORTSCALE, 20.0f, "cl_viewportscale", "");
	initVal(OPTION::GR_DEBUG, false, "gr_debug", "");
	initVal(OPTION::GR_FRAMESYNCMODE, 0, "gr_framesyncmode", "");
	initVal(OPTION::UI_SCALE, 1.0f, "ui_scale", "");
	initVal(OPTION::GR_RENDERTHREAD, false, "gr_renderthread", "seperate render and game logic threads");
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

			OPTION optionIndex = f->second;

			data[static_cast<size_t>(optionIndex)]->set(parts[2]);
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
