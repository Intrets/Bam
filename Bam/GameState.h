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
private:
	glm::vec2 playerCursorScreenSpace;
	glm::vec2 playerCursorWorldSpace;

public:
	bool exit;
	glm::vec2 playerPos;

	int tick = 0;

	glm::vec2 getPlayerCursorWorldSpace();
	void updatePlayerCursorScreenSpace(GLFWwindow* window);
	//GameObject* cam;
	//GameObject* player;

	//Inventory inventory;

	//ManagedReference<Activity, Activity> target;

	//std::vector<std::unique_ptr<GameObject>> objects;

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

