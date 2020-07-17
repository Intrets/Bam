#pragma once

#include <array>
#include <memory>

#include "StaticWorld.h"

#include "ActivityPaceHandler.h"
//#include "Inventory.h"
//#include "BaseWindow.h"
//#include "ReferenceManager.h"
#include "Saver.h"
#include "Loader.h"

//class GameObject;
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

