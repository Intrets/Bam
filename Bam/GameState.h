#pragma once

#include "StaticWorld.h"

#include "ActivityPaceHandler.h"
#include "Random.h"
#include "ReferenceManager.h"
#include "Activity.h"

class Saver;
class Loader;

struct RenderInfo;

class GameState
{
public:
	std::optional<std::string> loadFile = std::nullopt;
	std::optional<std::string> saveFile = std::nullopt;

	bool exit = false;
	glm::vec2 playerPos;

	int32_t tick = 0;

	StaticWorld staticWorld;
	ActivityPaceHandler activityPaceHandler;
	MovementPaceHandler movementPaceHandler;

	SmallRandom smallRandom;

	ReferenceManager<Activity> activityManager;

	bool load(Loader& loader);
	bool save(Saver& saver);

	void appendStaticRenderInfo(RenderInfo& renderInfo);

	GameState();
	~GameState();

	GameState(const GameState&) = delete;
	GameState& operator=(const GameState&) = delete;
};

