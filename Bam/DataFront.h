#pragma once

#include <vector>
#include <unordered_map>

#include "Saver.h"
#include "Loader.h"

template<class T>
class DataFront
{
private:
	static std::array<T, 1000> data;
	static std::array<std::string, 1000> names;
	static std::unordered_map<std::string, int32_t> nameMap;
	static int32_t size;

	int32_t index;

	friend void loadBlocks();
	friend void loadShapes();

public:
	static int32_t getDataID(std::string name);
	static std::vector<DataFront<T>> listAll();

	T const& getData() const;
	std::string const& getName() const;

	int32_t getID() const;

	DataFront(int32_t index_);
	DataFront(std::string name);
	DataFront();
	~DataFront() = default;

	bool load(Loader& loader);
	bool save(Saver& saver);
};

template<class T>
std::array<T, 1000> DataFront<T>::data{};

template<class T>
std::array<std::string, 1000> DataFront<T>::names{};

template<class T>
std::unordered_map<std::string, int32_t> DataFront<T>::nameMap{};

template<class T>
int32_t DataFront<T>::size{ 0 };

template<class T>
inline int32_t DataFront<T>::getDataID(std::string name) {
	auto it = DataFront<T>::nameMap.find(name);
	if (it != DataFront<T>::nameMap.end()) {
		return it->second;
	}
	else {
		return 0;
	}
}

template<class T>
inline std::vector<DataFront<T>> DataFront<T>::listAll() {
	std::vector<DataFront<T>> res;
	res.reserve(DataFront<T>::size);
	for (int32_t i = 0; i < DataFront<T>::size; i++) {
		res.push_back(DataFront<T>(i));
	}
	return res;
}

template<class T>
inline T const& DataFront<T>::getData() const {
	return DataFront<T>::data[this->index];
}

template<class T>
inline std::string const& DataFront<T>::getName() const {
	return DataFront<T>::names[this->index];
}

template<class T>
inline int32_t DataFront<T>::getID() const {
	return this->index;
}

template<class T>
inline DataFront<T>::DataFront(int32_t index_) : index(index_) {
}

template<class T>
inline DataFront<T>::DataFront(std::string name) {
	this->index = DataFront<T>::getDataID(name);
}

template<class T>
inline DataFront<T>::DataFront() : index(0) {
}

template<class T>
inline bool DataFront<T>::load(Loader& loader) {
	loader.retrieve(this->index);
	return true;
}

template<class T>
inline bool DataFront<T>::save(Saver& saver) {
	saver.store(this->index);
	return true;
}
