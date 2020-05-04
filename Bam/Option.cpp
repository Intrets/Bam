#include "common.h"

#include "Option.h"
#include <sstream>
#include <fstream>


void OptionManager::defaultValues() {
	initVal(OPTIONS2::CL_VIEWPORTSCALE, 20.0f);
	initVal(OPTIONS2::GR_DEBUG, true);
	initVal(OPTIONS2::GR_FRAMESYNCMODE, 0);
}

void OptionManager::readFromFile() {
	//std::ifstream file;

	//if (Locator<PathManager>::getService()->openFile(file, RESOURCE_FILE::OPTIONS)) {
	//	std::string line;
	//	while (std::getline(file, line)) {
	//		std::vector<std::string> parts;
	//		size_t i = 0;
	//		while (i < line.size()) {
	//			while (i < line.size() && line[i] == ' ') {
	//				i++;
	//			}
	//			int first = i;
	//			while (i < line.size() && line[i] != ' ') {
	//				i++;
	//			}
	//			int second = i;
	//			parts.push_back(line.substr(first, second - first));
	//		}
	//		if (parts.size() < 3) {
	//			continue;
	//		}
	//		try {
	//			if (parts[1] == "int") {
	//				int val = std::stoi(parts[2]);
	//				insert<int>(parts[0], val);
	//			}
	//			else if (parts[1] == "float") {
	//				float val = std::stof(parts[2]);
	//				insert<float>(parts[0], val);
	//			}
	//			else if (parts[1] == "bool") {
	//				bool val = std::stoi(parts[2]);
	//				insert<bool>(parts[0], val != 0);
	//			}
	//		}
	//		catch(std::exception){
	//			continue;
	//		}
	//	}
	//	file.close();
	//}
}

void OptionManager::writeToFile() {
	std::ofstream file;
	if (Locator<PathManager>::getService()->openFile(file, RESOURCE_FILE::OPTIONS)) {
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
