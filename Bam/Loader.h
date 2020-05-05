#pragma once

#include <fstream>

class GameState;

class Loader
{
public:
	int loc = 0;

	std::ifstream in;

	template<class T>
	bool retrieve(T& t);

	bool retrieveString(std::string& str);

	bool loadGame(GameState& gameState);

	Loader(std::string file);
	   
	Loader();
	~Loader();
};

template<class T>
inline bool Loader::retrieve(T & t) {
	in.read(reinterpret_cast<char*>(&t), sizeof(t));
	return false;
}

template<>
inline bool Loader::retrieve(std::string& t) {
	return retrieveString(t);
}
