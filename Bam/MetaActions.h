#pragma once

#include <vector>
#include <memory>

#include "MetaOperation.h"

class Loader;
class Saver;

class MetaActions
{
public:
	std::vector<std::unique_ptr<MetaOperation>> operations;

	void load(Loader& loader);
	void save(Saver& saver);
};

