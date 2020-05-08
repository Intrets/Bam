#include "common.h"

#include "ActivityPlacer.h"
#include "Platform.h"
#include "GameState.h"
#include <iostream>
#include "Piston.h"

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

void ActivityPlacer::exit(GameState& gameState) {
	if (hover) {
		hover.deleteObject();
	}
}

void ActivityPlacer::placeHover(GameState& gameState, glm::ivec2 pos) {
	if (hover) {
		if (hover.get()->fillTraces(gameState)) {
			hover.handle = 0;
		}
	}
	else {
		spawnHover(gameState, pos);
	}
}

void ActivityPlacer::spawnHover(GameState& gameState, glm::ivec2 pos) {
	if (hover) {
		hover.deleteObject();
	}
	switch (static_cast<HOVERTYPES>(hoverType)) {
		case HOVERTYPES::PLATFORM:
			hover = Locator<ReferenceManager<Activity>>::getService()->makeRef<Platform>(gameState, glm::ivec2(6, 5), pos, false);
			break;
		case HOVERTYPES::PISTON:
			hover = Locator<ReferenceManager<Activity>>::getService()->makeRef<Piston>(gameState, pos, MOVEABLE::UP, false);
			break;
		case HOVERTYPES::HOVERTYPES_MAX:
			break;
		default:
			break;
	}
}

ActivityPlacer::ActivityPlacer() {
	addBind({ CONTROLS::MOUSE_POS_CHANGED, CONTROLSTATE::CONTROLSTATE_UP }, &updateHoverPos);

	addBind({ CONTROLS::ACTION0, CONTROLSTATE::CONTROLSTATE_PRESSED }, &pickUpActivity);

	addBind({ CONTROLS::ACTION1, CONTROLSTATE::CONTROLSTATE_PRESSED }, [](GameState& gameState, LogicSequencer* self_) {
		auto self = static_cast<ActivityPlacer*>(self_);
		self->placeHover(gameState, gameState.getPlayerCursorWorldSpace());

		return std::make_pair(CONTINUATION::STOP, std::nullopt);
	});

	addBind({ CONTROLS::ACTION3, CONTROLSTATE::CONTROLSTATE_PRESSED }, [](GameState& gameState, LogicSequencer* self_) {
		auto self = static_cast<ActivityPlacer*>(self_);
		self->hoverType++;
		self->hoverType %= static_cast<int>(HOVERTYPES::HOVERTYPES_MAX);
		self->spawnHover(gameState, gameState.getPlayerCursorWorldSpace());

		return std::make_pair(CONTINUATION::STOP, std::nullopt);
	});

}

void ActivityPlacer::appendRenderInfoInternal(GameState& gameState, RenderInfo& renderInfo) {
	if (hover) {
		hover.get()->appendSelectionInfo(gameState, renderInfo);
	}
}

