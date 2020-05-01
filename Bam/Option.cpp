#include "common.h"

#include "Option.h"
#include <sstream>
#include <fstream>

std::optional<int> OptionManager::retrieveIndex(std::string name) {
	auto it = indexMap.find(name);
	if (it == indexMap.end()) {
		return {};
	}
	else {
		return it->second;
	}
}

std::string OptionManager::stringCommand(std::vector<std::string>& command) {
	if (command[0] == "saveoptions") {
		writeToFile();
		return "saved options";
	}
	if (command[0] == "list") {
		std::stringstream out;

		for (auto& p: data) {
			auto& ptr = p;
			out << ptr->toString() << "\n";
		}
		return out.str();
	}
	auto index = retrieveIndex(command[0]);
	if (!index) {
		return command[0] + " not found";
	}
	// retrieve value and print
	if (command.size() == 1) {
		return data[index.value()]->toString();
	}
	// set value
	else if (command.size() > 1) {
		return data[index.value()]->set(command[1]);
	}
	return "unable to parse";
}

void OptionManager::readFromFile() {
	std::ifstream file;

	if (Locator<PathManager>::getService()->openFile(file, RESOURCE_FILE::OPTIONS)) {
		std::string line;
		while (std::getline(file, line)) {
			std::vector<std::string> parts;
			size_t i = 0;
			while (i < line.size()) {
				while (i < line.size() && line[i] == ' ') {
					i++;
				}
				int first = i;
				while (i < line.size() && line[i] != ' ') {
					i++;
				}
				int second = i;
				parts.push_back(line.substr(first, second - first));
			}
			if (parts.size() < 3) {
				continue;
			}
			try {
				if (parts[1] == "int") {
					int val = std::stoi(parts[2]);
					insert<int>(parts[0], val);
				}
				else if (parts[1] == "float") {
					float val = std::stof(parts[2]);
					insert<float>(parts[0], val);
				}
				else if (parts[1] == "bool") {
					bool val = std::stoi(parts[2]);
					insert<bool>(parts[0], val != 0);
				}
			}
			catch(std::exception){
				continue;
			}
		}
		file.close();
	}
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
