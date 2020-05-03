#pragma once

#include <string>
#include <iostream>
#include <unordered_map>
#include <sstream>

#include "IOwrapped.h"
#include <optional>

class _OptionValueBase
{
private:
	std::string name;
	std::string description;

	friend class OptionManager;

public:
	std::string getName();
	std::string getDescription();
	std::string toString();
	virtual std::ostream& toStream(std::ostream& out) = 0;

	virtual std::string set(std::string) {
		return "read only";
	};
};

template <typename T>
class OptionValue : public _OptionValueBase
{
private:
	T val;
	
	friend class OptionManager;

public:
	T getVal();

	virtual std::string set(std::string) override;
	std::string type();

	virtual std::ostream & toStream(std::ostream& out) override {
		out << *this;
		return out;
	}
};

template<typename T>
inline std::ostream& operator<< (std::ostream& out, OptionValue<T>& D) {
	out << D.getName() << " " << D.type() << " " << D.getVal();
	return out;
}

template <typename T>
class Option
{
private:
	T defaultValue;

	int index = 0;
	const std::string name;

	friend class OptionManager;

public:
	operator T();
	void set(T val);

	Option(std::string name_, T defaultValue_);
	~Option() = default;
private:
	Option() = default;
};

class OptionManager
{
private:
	std::unordered_map<std::string, int> indexMap;
	std::vector<std::string> names;
	//std::unordered_map<int, std::unique_ptr<_OptionValueBase>> data;
	std::vector<std::unique_ptr<_OptionValueBase>> data;

public:
	template <class T>
	std::pair<bool, int> getIndex(std::string name);

	std::optional<int> retrieveIndex(std::string name);
	
	template <typename T>
	int insert(std::string name, T val);

	template <typename T>
	T getVal(Option<T>& option);

	template <typename T>
	void setVal(Option<T>& option, T val);

	std::string stringCommand(std::vector<std::string>& command);

	void readFromFile();
	void writeToFile();

	OptionManager();
	~OptionManager();
};

template<typename T>
inline Option<T>::operator T() {
	return Locator<OptionManager>::getService()->getVal(*this);
}

template<typename T>
inline void Option<T>::set(T val) {
	Locator<OptionManager>::getService()->setVal(*this, val);
}

template<typename T>
inline Option<T>::Option(std::string name_, T defaultValue_) : name(name_), defaultValue(defaultValue_) {
}

template<class T>
inline std::pair<bool, int> OptionManager::getIndex(std::string name) {
	int index;
	auto it = indexMap.find(name);
	bool newOption = it == indexMap.end();
	if (newOption) {
		index = names.size();
		indexMap[name] = index;
		names.push_back(name);
		data.emplace_back(new OptionValue<T>());
		data.back()->name = name;
	}
	else {
		index = it->second;
	}
	return std::make_pair(newOption, index);
}

template<typename T>
inline int OptionManager::insert(std::string name_, T val_) {
	auto pair = getIndex<T>(name_);
	int& index = pair.second;
	bool& newOption = pair.first;
	//static_cast<OptionValue<T>*>(data[index].get())->val = val_;
	if (newOption) {
		static_cast<OptionValue<T>*>(data[index].get())->val = val_;
	}
	return index;
}

template<typename T>
inline T OptionManager::getVal(Option<T>& option) {
	if (option.index == 0) {
		option.index = insert<T>(option.name, option.defaultValue);
	}
	return static_cast<OptionValue<T>*>(data[option.index].get())->val;
}

template<typename T>
inline void OptionManager::setVal(Option<T>& option, T val) {
	if (option.index == 0) {
		option.index = insert<T>(option.name, val);
	}
	else {
		static_cast<OptionValue<T>*>(data[option.index].get())->val = val;
	}
}

template<typename T>
inline T OptionValue<T>::getVal() {
	return val;
}

template<typename T>
inline std::string OptionValue<T>::set(std::string str) {
	std::stringstream in(str);
	try {
		in >> val;
		std::stringstream out;
		out << val;
		return "set value to: " + out.str();
	}
	catch (std::exception) {
		return "failed to parse input";
	}
}

template<>
inline std::string OptionValue<int>::type() {
	return "int";
}

template<>
inline std::string OptionValue<bool>::type() {
	return "bool";
}

template<>
inline std::string OptionValue<float>::type() {
	return "float";
}

template<>
inline std::string OptionValue<glm::vec2>::type() {
	return "vec2";
}

template<>
inline std::string OptionValue<glm::vec3>::type() {
	return "vec3";
}

template<>
inline std::string OptionValue<glm::vec4>::type() {
	return "vec4";
}

template<>
inline std::string OptionValue<glm::ivec2>::type() {
	return "ivec2";
}

template<>
inline std::string OptionValue<glm::ivec3>::type() {
	return "ivec3";
}

template<>
inline std::string OptionValue<glm::ivec4>::type() {
	return "ivec4";
}
