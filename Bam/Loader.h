#pragma once

#include <fstream>
#include "Activity.h"
#include "Piston.h"
#include "RailCrane.h"

#include <sol/sol.hpp>

class GameState;

class Loader
{
private:
	int32_t loc = 0;

	std::ifstream in;

public:
	template<class T>
	bool retrieve(T& t);

	template<class A, class B>
	bool retrieve(WeakReference<A, B>& m);

	sol::object retrieveObject(sol::state& state, std::unordered_map<size_t, sol::object>& cache);

	bool retrieveString(std::string& str);

	bool loadGame(GameState& gameState);

	Loader(std::string file);

	Loader();
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
inline bool Loader::retrieve(Activity::TYPE& t) {
	int32_t s;
	this->retrieve(s);
	t = static_cast<Activity::TYPE>(s);
	return true;
}

template<>
inline bool Loader::retrieve(Activity::DIR& t) {
	int32_t s;
	this->retrieve(s);
	t = static_cast<Activity::DIR>(s);
	return true;
}

template<>
inline bool Loader::retrieve(Piston::DIR& t) {
	int32_t s;
	this->retrieve(s);
	t = static_cast<Piston::DIR>(s);
	return true;
}

template<>
inline bool Loader::retrieve(RailCrane::DIR& t) {
	int32_t s;
	this->retrieve(s);
	t = static_cast<RailCrane::DIR>(s);
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
