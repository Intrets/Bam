#pragma once

#include <fstream>
#include "Activity.h"
#include "Piston.h"
#include "RailCrane.h"

#ifndef SOL_DEFINE
#define SOL_DEFINE
#include "sol/sol.hpp"
#endif 

class GameState;

class Loader
{
public:
	int32_t loc = 0;

	std::ifstream in;

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
	in.read(reinterpret_cast<char*>(&t), sizeof(t));
	return true;
}

template<>
inline bool Loader::retrieve(int32_t& t) {
	in.read(reinterpret_cast<char*>(&t), sizeof(t));
	return true;
}

template<>
inline bool Loader::retrieve(int8_t& t) {
	in.read(reinterpret_cast<char*>(&t), sizeof(t));
	return true;
}

template<>
inline bool Loader::retrieve(glm::ivec2& t) {
	int32_t x;
	retrieve(x);
	int32_t y;
	retrieve(y);
	t.x = static_cast<int>(x);
	t.y = static_cast<int>(y);
	return true;
}

template<>
inline bool Loader::retrieve(size_t& t) {
	int64_t s;
	retrieve(s);
	t = static_cast<size_t>(s);
	return true;
}

template<class A, class B>
inline bool Loader::retrieve(WeakReference<A, B>& m) {
	return retrieve(m.handle);
}

template<>
inline bool Loader::retrieve(bool& t) {
	int8_t s;
	retrieve(s);
	t = static_cast<bool>(s);
	return true;
}

template<>
inline bool Loader::retrieve(ACTIVITY::TYPE& t) {
	int32_t s;
	retrieve(s);
	t = static_cast<ACTIVITY::TYPE>(s);
	return true;
}

template<>
inline bool Loader::retrieve(MOVEABLE::DIR& t) {
	int32_t s;
	retrieve(s);
	t = static_cast<MOVEABLE::DIR>(s);
	return true;
}

template<>
inline bool Loader::retrieve(PISTON::DIR& t) {
	int32_t s;
	retrieve(s);
	t = static_cast<PISTON::DIR>(s);
	return true;
}

template<>
inline bool Loader::retrieve(RAILCRANE::DIR& t) {
	int32_t s;
	retrieve(s);
	t = static_cast<RAILCRANE::DIR>(s);
	return true;
}

template<>
inline bool Loader::retrieve(sol::type& t) {
	int32_t s;
	retrieve(s);
	t = static_cast<sol::type>(s);
	return true;
}

template<>
inline bool Loader::retrieve(double& t) {
	std::string s;
	retrieveString(s);
	t = std::stod(s);
	return true;
}

template<>
inline bool Loader::retrieve(std::string& t) {
	return retrieveString(t);
}

template<class T>
inline bool Loader::retrieve(T& t) {
	assert(false);
	//in.read(reinterpret_cast<char*>(&t), sizeof(t));
	return false;
}
