#pragma once

#include "StaticWorld.h"

#include "ActivityPaceHandler.h"
#include "Random.h"
#include "ReferenceManager.h"
#include "Activity.h"
#include "Player.h"
#include "InventoryItem.h"

class Saver;
class Loader;

struct RenderInfo;

class GameState
{
private:
	ReferenceManager<Activity> activityManager;
	ReferenceManager<InventoryItem> inventoryItemManager;

	std::vector<std::unique_ptr<Player>> players;

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

	ReferenceManager<Activity>& getActivityManager();
	ReferenceManager<InventoryItem>& getInventoryItemManager();

	std::optional<Player*> getPlayer(int32_t index);
	void makePlayer();

	bool load(Loader& loader);
	bool save(Saver& saver);

	void appendStaticRenderInfo(RenderInfo& renderInfo);

	GameState();
	~GameState();

	GameState(const GameState&) = delete;
	GameState& operator=(const GameState&) = delete;
};

