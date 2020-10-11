#pragma once

#include <fstream>
#include <sol/sol.hpp>
#include <optional>

#include "Enums.h"

class GameState;

template<class, class>
class WeakReference;

class Loader
{
private:
	int32_t loc = 0;

	std::ifstream in;

	GameState& gameStateRef;

public:
	template<class T>
	bool retrieve(T& t);

	template<class T>
	bool retrieve(std::optional<T>& m);

	template<class A, class B>
	bool retrieve(WeakReference<A, B>& m);

	sol::object retrieveObject(sol::state& state, std::unordered_map<size_t, sol::object>& cache);

	bool retrieveString(std::string& str);

	bool loadGame();
	GameState& getGameStateRef();

	Loader(std::string file, GameState& gameState);

	Loader() = delete;
	~Loader();
};

template<>
inline bool Loader::retrieve(int64_t& t) {
	this->in.read(reinterpret_cast<char*>(&t), sizeof(t));
	return true;
}

template<>
inline bool Loader::retrieve(int32_t& t) {
	this->in.read(reinterpret_cast<char*>(&t), sizeof(t));
	return true;
}

template<>
inline bool Loader::retrieve(int8_t& t) {
	this->in.read(reinterpret_cast<char*>(&t), sizeof(t));
	return true;
}

template<>
inline bool Loader::retrieve(glm::ivec2& t) {
	int32_t x;
	this->retrieve(x);
	int32_t y;
	this->retrieve(y);
	t.x = static_cast<int>(x);
	t.y = static_cast<int>(y);
	return true;
}

template<>
inline bool Loader::retrieve(size_t& t) {
	int64_t s;
	this->retrieve(s);
	t = static_cast<size_t>(s);
	return true;
}

template<class A, class B>
inline bool Loader::retrieve(WeakReference<A, B>& m) {
	return this->retrieve(m.handle);
}

template<>
inline bool Loader::retrieve(bool& t) {
	int8_t s;
	this->retrieve(s);
	t = static_cast<bool>(s);
	return true;
}

template<>
inline bool Loader::retrieve(ACTIVITY::TYPE& t) {
	int32_t s;
	this->retrieve(s);
	t = static_cast<ACTIVITY::TYPE>(s);
	return true;
}

template<>
inline bool Loader::retrieve(ACTIVITY::DIR& t) {
	int32_t s;
	this->retrieve(s);
	t = static_cast<ACTIVITY::DIR>(s);
	return true;
}

template<>
inline bool Loader::retrieve(PISTON::DIR& t) {
	int32_t s;
	this->retrieve(s);
	t = static_cast<PISTON::DIR>(s);
	return true;
}

template<>
inline bool Loader::retrieve(RAILCRANE::DIR& t) {
	int32_t s;
	this->retrieve(s);
	t = static_cast<RAILCRANE::DIR>(s);
	return true;
}

template<>
inline bool Loader::retrieve(INVENTORYITEM::TYPE& t) {
	int32_t s;
	this->retrieve(s);
	t = static_cast<INVENTORYITEM::TYPE>(s);
	return true;
}

template<>
inline bool Loader::retrieve(sol::type& t) {
	int32_t s;
	this->retrieve(s);
	t = static_cast<sol::type>(s);
	return true;
}

template<>
inline bool Loader::retrieve(double& t) {
	std::string s;
	this->retrieveString(s);
	t = std::stod(s);
	return true;
}

template<>
inline bool Loader::retrieve(std::string& t) {
	return this->retrieveString(t);
}

template<class T>
inline bool Loader::retrieve(T& t) {
	assert(false);
	//in.read(reinterpret_cast<char*>(&t), sizeof(t));
	return false;
}

template<class T>
inline bool Loader::retrieve(std::optional<T>& m) {
	bool hasValue;
	retrieve(hasValue);
	if (hasValue) {
		m = {};
		m.value().load(*this);
	}
	return true;
}
