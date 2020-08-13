#pragma once

#include "StaticWorld.h"

#include "ActivityPaceHandler.h"

class Saver;
class Loader;

struct RenderInfo;

class GameState
{
public:
	bool exit;
	glm::vec2 playerPos;

	int32_t tick = 0;

	StaticWorld staticWorld;
	ActivityPaceHandler activityPaceHandler;
	MovementPaceHandler movementPaceHandler;

	bool load(Loader& loader);
	bool save(Saver& saver);

	void appendStaticRenderInfo(RenderInfo& renderInfo);

	GameState();
	~GameState();

	GameState(const GameState&) = delete;
	GameState& operator=(const GameState&) = delete;
};

