#pragma once

#include "NetworkTypes.h"

class GameState;

struct Buffer
{
	size_t size;
	void* danger;
};

struct ReadingBuffer
{
	size_t size;
	size_t offset;
	void* danger;

	int32_t nextInt32();
	std::string nextString();
};

struct Operation
{
	NWT::TYPE type;
	size_t method;

	Buffer args;

	void run(GameState& gameState);
};

class Network
{
public:
	std::vector<Operation> operations;
};

