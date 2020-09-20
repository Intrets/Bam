#pragma once

#include "Activity.h"

template<class T, class R>
class UniqueReference;

class LuaActivity;

class GameState;

namespace ACTIVITYSPAWNER
{
	std::optional<UniqueReference<Activity, Activity>> spawn(GameState& gameState, glm::ivec2 pos, ACTIVITY::TYPE activityType);

	UniqueReference<Activity, Activity> piston(GameState& gameState, glm::ivec2 pos);
	UniqueReference<Activity, Activity> railcrane(GameState& gameState, glm::ivec2 pos);
	UniqueReference<Activity, Activity> platform(GameState& gameState, glm::ivec2 pos);
	UniqueReference<Activity, Activity> lua(GameState& gameState, glm::ivec2 pos);
	UniqueReference<Activity, Activity> grabber(GameState& gameState, glm::ivec2 pos);
	UniqueReference<Activity, Activity> reader(GameState& gameState, glm::ivec2 pos);
	UniqueReference<Activity, Activity> detector(GameState& gameState, glm::ivec2 pos);
	UniqueReference<Activity, Activity> incinerator(GameState& gameState, glm::ivec2 pos);
	UniqueReference<Activity, Activity> forwarder(GameState& gameState, glm::ivec2 pos);
};

