#pragma once

#include <fstream>

class GameState;

class Saver
{
public:
	std::ofstream out;

	template<class T>
	bool store(T t);

	bool storeString(std::string s);

	bool saveGame(GameState& gameState);

	void close();

	Saver(std::string file);
	Saver();
	~Saver();
};

template<>
inline bool Saver::store(std::string t) {
	return storeString(t);
}

template<class T>
inline bool Saver::store(T t) {
	out.write(reinterpret_cast<char*>(&t), sizeof(t));
	return true;
}

