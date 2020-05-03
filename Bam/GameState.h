#pragma once

#include <array>
#include <memory>

#include "StaticWorld.h"

//#include "ActivityPaceHandler.h"
//#include "Inventory.h"
//#include "BaseWindow.h"
//#include "ReferenceManager.h"
//#include "Saver.h"
//#include "Loader.h"

//class GameObject;
class RenderInfo;

class GameState
{
public:
	bool exit;

	int tick = 0;
	//GameObject* cam;
	//GameObject* player;
	glm::vec2 playerCursor;
	glm::vec2 playerPos;

	//Inventory inventory;

	//ManagedReference<Activity, Activity> target;

	//std::vector<std::unique_ptr<GameObject>> objects;

	StaticWorld staticWorld;
	//ActivityPaceHandler activityPaceHandler;
	//MovementPaceHandler movementPaceHandler;

	//bool load(Loader& loader);
	//bool save(Saver& saver);

	void appendStaticRenderInfo(RenderInfo& renderInfo);

	GameState();
	~GameState();

	GameState(const GameState&) = delete;
	GameState& operator=(const GameState&) = delete;
};

