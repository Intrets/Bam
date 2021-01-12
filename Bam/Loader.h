#pragma once

#include <fstream>
#include <sol/sol.hpp>
#include <optional>

#include "Enums.h"

#include "ReferenceManager.h"

class GameState;
class Activity;
class InventoryItem;

class Loader
{
private:
	int32_t loc = 0;

	std::istream& in;

	GameState* gameStateRef = nullptr;

	void addIncompleteActivityRef(Handle handle, Reference* ref);
	void addIncompleteInventoryRef(Handle handle, Reference* ref);

public:
	std::istream& getBuffer();

	void retrieveActivityPointer(Activity*& ptr);

	template<class T>
	void retrieve(T& t);

	template<class T>
	void retrieve(std::optional<T>& m);

	template<class T>
	void retrieve(WeakReference<Activity, T>& m);

	template<class T>
	void retrieve(UniqueReference<Activity, T>& m);

	template<class T>
	void retrieve(ManagedReference<Activity, T>& m);

	template<class T>
	void retrieve(WeakReference<InventoryItem, T>& m);

	template<class T>
	void retrieve(UniqueReference<InventoryItem, T>& m);

	template<class T>
	void retrieve(ManagedReference<InventoryItem, T>& m);

	sol::object retrieveObject(sol::state& state, std::unordered_map<size_t, sol::object>& cache);

	void retrieveString(std::string& str);

	bool loadGame();
	GameState& getGameStateRef();

	// sets flags so the stream will throw exceptions
	Loader(std::istream& in_, GameState& gameState);
	Loader(std::istream& in_);

	Loader() = delete;
	~Loader();
};

template<>
inline void Loader::retrieve(int64_t& t) {
	this->in.read(reinterpret_cast<char*>(&t), sizeof(t));
}

template<>
inline void Loader::retrieve(int32_t& t) {
	this->in.read(reinterpret_cast<char*>(&t), sizeof(t));
}

template<>
inline void Loader::retrieve(int8_t& t) {
	this->in.read(reinterpret_cast<char*>(&t), sizeof(t));
}

template<>
inline void Loader::retrieve(glm::ivec2& t) {
	int32_t x;
	this->retrieve(x);
	int32_t y;
	this->retrieve(y);
	t.x = static_cast<int>(x);
	t.y = static_cast<int>(y);
}

template<>
inline void Loader::retrieve(size_t& t) {
	int64_t s;
	this->retrieve(s);
	t = static_cast<size_t>(s);
}

template<class T>
inline void Loader::retrieve(WeakReference<Activity, T>& m) {
	Handle handle;
	this->retrieve(handle);
	if (handle != 0) {
		this->addIncompleteActivityRef(handle, &m);
	}
	else {
		m.clear();
	}
}

template<class T>
inline void Loader::retrieve(UniqueReference<Activity, T>& m) {
	Handle handle;
	this->retrieve(handle);
	if (handle != 0) {
		this->addIncompleteActivityRef(handle, &m);
	}
	else {
		m.ptr = nullptr;
	}
}

template<class T>
inline void Loader::retrieve(ManagedReference<Activity, T>& m) {
	Handle handle;
	this->retrieve(handle);
	if (handle != 0) {
		this->addIncompleteActivityRef(handle, &m);
	}
	else {
		m.ptr = nullptr;
	}
}

template<class T>
inline void Loader::retrieve(WeakReference<InventoryItem, T>& m) {
	Handle handle;
	this->retrieve(handle);
	if (handle != 0) {
		this->addIncompleteInventoryRef(handle, &m);
	}
	else {
		m.clear();
	}
}

template<class T>
inline void Loader::retrieve(UniqueReference<InventoryItem, T>& m) {
	Handle handle;
	this->retrieve(handle);
	if (handle != 0) {
		this->addIncompleteInventoryRef(handle, &m);
	}
	else {
		m.ptr = nullptr;
	}
}

template<class T>
inline void Loader::retrieve(ManagedReference<InventoryItem, T>& m) {
	Handle handle;
	this->retrieve(handle);
	if (handle != 0) {
		this->addIncompleteInventoryRef(handle, &m);
	}
	else {
		m.ptr = nullptr;
	}
}

template<>
inline void Loader::retrieve(bool& t) {
	int8_t s;
	this->retrieve(s);
	t = static_cast<bool>(s);
}

template<>
inline void Loader::retrieve(ACTIVITY::TYPE& t) {
	int32_t s;
	this->retrieve(s);
	t = static_cast<ACTIVITY::TYPE>(s);
}

template<>
inline void Loader::retrieve(ACTIVITY::DIR& t) {
	int32_t s;
	this->retrieve(s);
	t = static_cast<ACTIVITY::DIR>(s);
}

template<>
inline void Loader::retrieve(PISTON::DIR& t) {
	int32_t s;
	this->retrieve(s);
	t = static_cast<PISTON::DIR>(s);
}

template<>
inline void Loader::retrieve(RAILCRANE::DIR& t) {
	int32_t s;
	this->retrieve(s);
	t = static_cast<RAILCRANE::DIR>(s);
}

template<>
inline void Loader::retrieve(INVENTORYITEM::TYPE& t) {
	int32_t s;
	this->retrieve(s);
	t = static_cast<INVENTORYITEM::TYPE>(s);
}

template<>
inline void Loader::retrieve(sol::type& t) {
	int32_t s;
	this->retrieve(s);
	t = static_cast<sol::type>(s);
}

template<>
inline void Loader::retrieve(double& t) {
	std::string s;
	this->retrieveString(s);
	t = std::stod(s);
}

template<>
inline void Loader::retrieve(glm::vec2& t) {
	double x;
	this->retrieve(x);
	double y;
	this->retrieve(y);
	t.x = static_cast<float>(x);
	t.y = static_cast<float>(y);
}

template<>
inline void Loader::retrieve(std::string& t) {
	this->retrieveString(t);
}

template<class T>
inline void Loader::retrieve(T& t) {
	assert(false);
}

template<class T>
inline void Loader::retrieve(std::optional<T>& m) {
	bool hasValue;
	this->retrieve(hasValue);
	if (hasValue) {
		m = T();
		m.value().load(*this);
	}
}
