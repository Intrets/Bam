#pragma once

#include "Activity.h"

template<class T, class R>
class UniqueReference;

class LUAActivity;

class GameState;

namespace ACTIVITYSPAWNER
{
	std::optional<UniqueReference<Activity, Activity>> spawn(GameState& gameState, glm::ivec2 pos, Activity::TYPE activityType);

	UniqueReference<Activity, Activity> piston(GameState& gameState, glm::ivec2 pos);
	UniqueReference<Activity, Activity> railcrane(GameState& gameState, glm::ivec2 pos);
	UniqueReference<Activity, Activity> platform(GameState& gameState, glm::ivec2 pos);

	WeakReference<Activity, LUAActivity> addLUA(GameState& gameState, WeakReference<Activity,Activity> ref);
};

