#include "common.h"

#include "ActivitySpawner.h"
#include "Platform.h"
#include "Piston.h"
#include "RailCrane.h"
#include "LuaActivity.h"

std::optional<UniqueReference<Activity, Activity>> ACTIVITYSPAWNER::spawn(GameState& gameState, glm::ivec2 pos, Activity::TYPE activityType) {
	switch (activityType) {
		case Activity::TYPE::ANCHOR:
			return std::nullopt;
			break;
		case Activity::TYPE::MOVER:
			return std::nullopt;
			break;
		case Activity::TYPE::PLATFORM:
			return ACTIVITYSPAWNER::platform(gameState, pos);
			break;
		case Activity::TYPE::PISTON:
			return ACTIVITYSPAWNER::piston(gameState, pos);
			break;
		case Activity::TYPE::SINGLEPLATFORM:
			return std::nullopt;
			break;
		case Activity::TYPE::BREAKER:
			return std::nullopt;
			break;
		case Activity::TYPE::GRABBER:
			return std::nullopt;
			break;
		case Activity::TYPE::PLANT:
			return std::nullopt;
			break;
		case Activity::TYPE::ACTIVATOR:
			return std::nullopt;
			break;
		case Activity::TYPE::RAILCRANE:
			return ACTIVITYSPAWNER::railcrane(gameState, pos);
			break;
		case Activity::TYPE::_MAX:
			return std::nullopt;
			break;
		case Activity::TYPE::LUA:
			return ACTIVITYSPAWNER::lua(gameState, pos);
			break;
		default:
			return std::nullopt;
			break;
	}
}

UniqueReference<Activity, Activity> ACTIVITYSPAWNER::piston(GameState& gameState, glm::ivec2 pos) {
	return Locator<ReferenceManager<Activity>>::get()->makeUniqueRef<Piston>(gameState, pos, Activity::DIR::DOWN, false);
}

UniqueReference<Activity, Activity> ACTIVITYSPAWNER::railcrane(GameState& gameState, glm::ivec2 pos) {
	return Locator<ReferenceManager<Activity>>::get()->makeUniqueRef<RailCrane>(gameState, pos, false);
}

UniqueReference<Activity, Activity> ACTIVITYSPAWNER::platform(GameState& gameState, glm::ivec2 pos) {
	return Locator<ReferenceManager<Activity>>::get()->makeUniqueRef<Platform>(gameState, glm::ivec2(6, 5), pos, false);
}

UniqueReference<Activity, Activity> ACTIVITYSPAWNER::lua(GameState& gameState, glm::ivec2 pos) {
	return Locator<ReferenceManager<Activity>>::get()->makeUniqueRef<LuaActivity>(gameState, pos, false);
}
