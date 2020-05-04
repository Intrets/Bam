#pragma once

#include <string>
#include <iostream>
#include <unordered_map>
#include <sstream>

#include "IOwrapped.h"
#include <optional>

class OptionManager;

typedef enum
{
	GR_FRAMESYNCMODE,
	GR_DEBUG,
	CL_VIEWPORTSCALE,
	OPTIONS2_MAX
} OPTIONS2;

class _OptionBase
{

};

template<OPTIONS2 A, class T>
class Option : public _OptionBase
{
public:
	T getVal();
	void setVal(T val);
};

template<OPTIONS2 A, class T>
inline T Option<A, T>::getVal() {
	return Locator<OptionManager>::getService()->getVal<T>(A);
}

template<OPTIONS2 A, class T>
inline void Option<A, T>::setVal(T val) {
	Locator<OptionManager>::getService()->setVal(A, val);
}

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
	OptionValue(T val_) : val(val_) {};

	T getVal() { return val; };
	void setVal(T val_) { val = val_; };

	virtual std::string set(std::string) override;
	std::string type();

	virtual std::ostream& toStream(std::ostream& out) override {
		out << *this;
		return out;
	}
};

template<typename T>
inline std::ostream& operator<< (std::ostream& out, OptionValue<T>& D) {
	out << D.getName() << " " << D.type() << " " << D.getVal();
	return out;
}

class OptionManager
{
private:
	//std::unordered_map<std::string, int> indexMap;
	//std::vector<std::string> names;
	//std::unordered_map<int, std::unique_ptr<_OptionValueBase>> data;
	//std::vector<std::unique_ptr<_OptionValueBase>> data;
	//std::vector<std::unique_ptr<_OptionValueBase>> data;
	std::array<std::unique_ptr<_OptionValueBase>, OPTIONS2::OPTIONS2_MAX> data;

	void defaultValues();

	template<class T>
	void initVal(OPTIONS2 option, T val);

public:
	template<class T>
	T getVal(OPTIONS2 option);

	template<class T>
	void setVal(OPTIONS2 option, T val);

	//template <typename T>
	//T getVal(Option<T>& option);

	//template <typename T>
	//void setVal(Option<T>& option, T val);

	//std::string stringCommand(std::vector<std::string>& command);

	void readFromFile();
	void writeToFile();

	OptionManager();
	~OptionManager();
};

template<class T>
inline T OptionManager::getVal(OPTIONS2 option) {
	return static_cast<OptionValue<T>*>(data[option].get())->val;
}

template<class T>
inline void OptionManager::setVal(OPTIONS2 option, T val) {
	static_cast<OptionValue<T>*>(data[option].get())->setVal(val);
}

template<class T>
inline void OptionManager::initVal(OPTIONS2 option, T val) {
	data[option] = std::make_unique<OptionValue<T>>(val);
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
