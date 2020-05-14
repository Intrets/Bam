#pragma once

#include <fstream>
#include "Activity.h"
#include "Piston.h"
#include "RailCrane.h"

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
