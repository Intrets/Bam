#include "common.h"

#include "ActivitySpawner.h"
#include "Platform.h"
#include "Piston.h"
#include "RailCrane.h"
#include "LuaActivity.h"
#include "Grabber.h"
#include "Reader.h"
#include "Detector.h"
#include "Incinerator.h"

const char* initialLuaScript =
R"rawstring(
-- send(index, ...) 
-- move(index, int32_t)
-- activate(index, int32_t)

function init()
  state = state or 0
end

stateTable = {
  [0] = function()
    print(0)
    state = 1
  end,
  [1] = function()
    print(1)
    state = 0
    stop()
  end,
}

function run()
end

function message(...)
  print("message received");
end
)rawstring";

std::optional<UniqueReference<Activity, Activity>> ACTIVITYSPAWNER::spawn(GameState& gameState, glm::ivec2 pos, ACTIVITY::TYPE activityType) {
	switch (activityType) {
		case ACTIVITY::TYPE::ANCHOR:
			return std::nullopt;
			break;
		case ACTIVITY::TYPE::MOVER:
			return std::nullopt;
			break;
		case ACTIVITY::TYPE::PLATFORM:
			return ACTIVITYSPAWNER::platform(gameState, pos);
			break;
		case ACTIVITY::TYPE::PISTON:
			return ACTIVITYSPAWNER::piston(gameState, pos);
			break;
		case ACTIVITY::TYPE::SINGLEPLATFORM:
			return std::nullopt;
			break;
		case ACTIVITY::TYPE::BREAKER:
			return std::nullopt;
			break;
		case ACTIVITY::TYPE::GRABBER:
			return ACTIVITYSPAWNER::grabber(gameState, pos);
			break;
		case ACTIVITY::TYPE::PLANT:
			return std::nullopt;
			break;
		case ACTIVITY::TYPE::RAILCRANE:
			return ACTIVITYSPAWNER::railcrane(gameState, pos);
			break;
		case ACTIVITY::TYPE::_MAX:
			return std::nullopt;
			break;
		case ACTIVITY::TYPE::LUA:
			return ACTIVITYSPAWNER::lua(gameState, pos);
			break;
		case ACTIVITY::TYPE::READER:
			return ACTIVITYSPAWNER::reader(gameState, pos);
			break;
		case ACTIVITY::TYPE::DETECTOR:
			return ACTIVITYSPAWNER::detector(gameState, pos);
			break;
		case ACTIVITY::TYPE::INCINERATOR:
			return ACTIVITYSPAWNER::incinerator(gameState, pos);
			break;
		default:
			return std::nullopt;
			break;
	}
}

UniqueReference<Activity, Activity> ACTIVITYSPAWNER::piston(GameState& gameState, glm::ivec2 pos) {
	return Locator<ReferenceManager<Activity>>::get()->makeUniqueRef<Piston>(pos, ACTIVITY::DIR::DOWN);
}

UniqueReference<Activity, Activity> ACTIVITYSPAWNER::railcrane(GameState& gameState, glm::ivec2 pos) {
	return Locator<ReferenceManager<Activity>>::get()->makeUniqueRef<RailCrane>(gameState, pos);
}

UniqueReference<Activity, Activity> ACTIVITYSPAWNER::platform(GameState& gameState, glm::ivec2 pos) {
	return Locator<ReferenceManager<Activity>>::get()->makeUniqueRef<Platform>(gameState, glm::ivec2(6, 5), pos);
}

UniqueReference<Activity, Activity> ACTIVITYSPAWNER::lua(GameState& gameState, glm::ivec2 pos) {
	auto ref = Locator<ReferenceManager<Activity>>::get()->makeUniqueRef<LuaActivity>(gameState, pos);
	ref.get()->setScript(initialLuaScript, gameState);
	return std::move(ref);
}

UniqueReference<Activity, Activity> ACTIVITYSPAWNER::grabber(GameState& gameState, glm::ivec2 pos) {
	return Locator<ReferenceManager<Activity>>::get()->makeUniqueRef<Grabber>(pos);
}

UniqueReference<Activity, Activity> ACTIVITYSPAWNER::reader(GameState& gameState, glm::ivec2 pos) {
	return Locator<ReferenceManager<Activity>>::get()->makeUniqueRef<Reader>(pos);
}

UniqueReference<Activity, Activity> ACTIVITYSPAWNER::detector(GameState& gameState, glm::ivec2 pos) {
	return Locator<ReferenceManager<Activity>>::get()->makeUniqueRef<Detector>(pos);
}

UniqueReference<Activity, Activity> ACTIVITYSPAWNER::incinerator(GameState& gameState, glm::ivec2 pos) {
	return Locator<ReferenceManager<Activity>>::get()->makeUniqueRef<Incinerator>(pos);
}
