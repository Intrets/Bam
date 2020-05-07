#include "common.h"

#include "ActivityPlacer.h"
#include "Platform.h"
#include "GameState.h"
#include <iostream>

static auto updateHoverPos(GameState& gameState, LogicSequencer* self_) {
	auto self = static_cast<ActivityPlacer*>(self_);
	if (self->hover) {
		self->hover.get()->forceOrigin(gameState.getPlayerCursorWorldSpace());
	}
	return std::make_pair(CONTINUATION::CONTINUE, std::nullopt);
}

static auto pickUpActivity(GameState& gameState, LogicSequencer* self_) {
	auto self = static_cast<ActivityPlacer*>(self_);
	if (!self->hover) {
		auto target = gameState.staticWorld.getBlock(gameState.getPlayerCursorWorldSpace());
		if (target.first == 1) {
			if (target.second.get()->removeTraces(gameState)) {
				self->hover = target.second;
			}
		}
	}

	return std::make_pair(CONTINUATION::CONTINUE, std::nullopt);
}

void ActivityPlacer::placeHover(GameState& gameState, glm::ivec2 pos) {
	if (hover) {
		hover.get()->fillTraces(gameState);
		hover.handle = 0;
	}
	else {
		hover = Locator<ReferenceManager<Activity>>::getService()->makeRef<Platform>(gameState, glm::ivec2(2, 3), pos, false);
	}
}


ActivityPlacer::ActivityPlacer() {
	addBind({ CONTROLS::MOUSE_POS_CHANGED, CONTROLSTATE::CONTROLSTATE_UP }, &updateHoverPos);

	addBind({ CONTROLS::SELECTACTIVITY, CONTROLSTATE::CONTROLSTATE_PRESSED }, &pickUpActivity);

	addBind({ CONTROLS::PLACEBLOCK_HOVER, CONTROLSTATE::CONTROLSTATE_PRESSED }, [](GameState& gameState, LogicSequencer* self_) {
		auto self = static_cast<ActivityPlacer*>(self_);
		self->placeHover(gameState, gameState.getPlayerCursorWorldSpace());

		return std::make_pair(CONTINUATION::STOP, std::nullopt);

	});

}
