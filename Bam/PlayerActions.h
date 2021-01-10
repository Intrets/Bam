#pragma once

#include "Network.h"

#include <vector>
#include <memory>

class Loader;
class Saver;

class PlayerActions
{
public:
	std::vector<std::unique_ptr<Operation>> operations;

	void load(Loader& loader);
	void save(Saver& saver);
};

