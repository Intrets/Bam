#pragma once

#include <fstream>
#include "Activity.h"
#include "Piston.h"
#include "RailCrane.h"
#include "sol/sol.hpp"

class GameState;

class Saver
{
public:
	std::ofstream out;

	template<class T>
	bool store(T t);

	template<class A, class B>
	bool store(WeakReference<A, B> t);

	bool storeString(std::string s);

	bool saveGame(GameState& gameState);

	void close();

	Saver(std::string file);
	Saver();
	~Saver();
};

template<>
inline bool Saver::store(int64_t t) {
	out.write(reinterpret_cast<char*>(&t), sizeof(t));
	return true;
}

template<>
inline bool Saver::store(int32_t t) {
	out.write(reinterpret_cast<char*>(&t), sizeof(t));
	return true;
}

template<>
inline bool Saver::store(int8_t t) {
	out.write(reinterpret_cast<char*>(&t), sizeof(t));
	return true;
}

template<>
inline bool Saver::store(size_t t) {
	store(static_cast<int64_t>(t));
	return true;
}

template<>
inline bool Saver::store(glm::ivec2 t) {
	store(static_cast<int32_t>(t.x));
	store(static_cast<int32_t>(t.y));
	return true;
}

template<>
inline bool Saver::store(bool t) {
	int8_t s = static_cast<int8_t>(t);
	store(s);
	return true;
}

template<>
inline bool Saver::store(ACTIVITY::TYPE t) {
	int32_t s = static_cast<int32_t>(t);
	store(s);
	return true;
}

template<>
inline bool Saver::store(MOVEABLE::DIR t) {
	int32_t s = static_cast<int32_t>(t);
	store(s);
	return true;
}

template<>
inline bool Saver::store(PISTON::DIR t) {
	int32_t s = static_cast<int32_t>(t);
	store(s);
	return true;
}

template<>
inline bool Saver::store(RAILCRANE::DIR t) {
	int32_t s = static_cast<int32_t>(t);
	store(s);
	return true;
}

template<>
inline bool Saver::store(std::string t) {
	return storeString(t);
}

template<>
inline bool Saver::store(sol::type type) {
	return store(static_cast<int32_t>(type));
}

template<>
inline bool Saver::store(sol::object t) {
	auto type = t.get_type();
	if (type == sol::type::boolean) {
		store(type);
		store(t.as<bool>());
	}
	else if (type == sol::type::number) {
		store(type);
		storeString(std::to_string(t.as<double>()));
	}
	else if (type == sol::type::string) {
		store(type);
		storeString(t.as<std::string>());
	}
	else {
		assert(false);
	}
	return true;
}

template<class T>
inline bool Saver::store(T t) {
	assert(false);
	//out.write(reinterpret_cast<char*>(&t), sizeof(t));
	return true;
}

template<class A, class B>
inline bool Saver::store(WeakReference<A, B> t) {
	store(t.handle);
	return true;
}
