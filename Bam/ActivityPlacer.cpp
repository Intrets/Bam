#include "common.h"

#include "ActivityPlacer.h"
#include "Platform.h"
#include "GameState.h"
#include <iostream>
#include "Piston.h"

static auto updateHoverPos(GameState& gameState, LogicSequencer* self_) {
	auto self = static_cast<ActivityPlacer*>(self_);
	if (self->hover.isNotNull()) {
		auto pos = glm::ivec2(glm::floor(gameState.getPlayerCursorWorldSpace()));
		self->hover.get()->forceMoveOriginLocal(pos - self->hover.get()->getOrigin());
	}
	return std::make_pair(CONTINUATION::CONTINUE, std::nullopt);
}

static auto pickUpActivity(GameState& gameState, LogicSequencer* self_) {
	auto self = static_cast<ActivityPlacer*>(self_);
	if (self->target.isValid()) {
		if (self->target.get()->idleLocal()) {
			self->hover = WeakReference<Activity, Activity>(self->target.handle);
			self->hover.get()->removeTracesUp(gameState);
			self->target.unset();
		}
	}
	else if (self->hover.isNotNull()) {
		if (self->hover.get()->fillTracesUp(gameState)) {
			self->hover.handle = 0;
		}
	}
	else {
		self->spawnHover(gameState, gameState.getPlayerCursorWorldSpace());
	}
	return std::make_pair(CONTINUATION::CONTINUE, std::nullopt);
}

void ActivityPlacer::exit(GameState& gameState) {
	deleteHover();
}

void ActivityPlacer::selectTarget(GameState& gameState) {
	if (hover.isNull()) {
		ActivitySelector::selectTarget(gameState);
	}
}

void ActivityPlacer::spawnHover(GameState& gameState, glm::ivec2 pos) {
	if (hover.isNotNull()) {
		deleteHover();
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

void ActivityPlacer::rotateHover(MOVEABLE::ROT rot) {
	if (hover.isNotNull()) {
		auto center = hover.get()->getOrigin();
		hover.get()->rotateForcedUp(center, rot);
	}
}

void ActivityPlacer::deleteHover() {
	if (hover.isNotNull()) {
		auto activity = hover.get();
		activity->disconnectFromParent();
		std::vector<Activity*> members;
		activity->getTreeMembers(members);
		for (auto member : members) {
			WeakReference<Activity, Activity>(member->selfHandle).deleteObject();
		}
	}
}

ActivityPlacer::ActivityPlacer() {
	addBind({ CONTROLS::MOUSE_POS_CHANGED, CONTROLSTATE::CONTROLSTATE_UP }, &updateHoverPos);

	addBind({ CONTROLS::ACTION2, CONTROLSTATE::CONTROLSTATE_PRESSED }, &pickUpActivity);

	addBind({ CONTROLS::ACTION3, CONTROLSTATE::CONTROLSTATE_PRESSED }, [](GameState& gameState, LogicSequencer* self_) {
		auto self = static_cast<ActivityPlacer*>(self_);
		if (!self->target.isValid()) {
			self->hoverType++;
			self->hoverType %= static_cast<int>(HOVERTYPES::HOVERTYPES_MAX);
			self->spawnHover(gameState, gameState.getPlayerCursorWorldSpace());
		}

		return std::make_pair(CONTINUATION::STOP, std::nullopt);
	});

	addBind({ CONTROLS::ROTATEL, CONTROLSTATE::CONTROLSTATE_PRESSED }, [](GameState& gameState, LogicSequencer* self_) {
		auto self = static_cast<ActivityPlacer*>(self_);
		self->rotateHover(MOVEABLE::ROT::COUNTERCLOCKWISE);
		return std::make_pair(CONTINUATION::CONTINUE, std::nullopt);
	});

	addBind({ CONTROLS::ROTATER, CONTROLSTATE::CONTROLSTATE_PRESSED }, [](GameState& gameState, LogicSequencer* self_) {
		auto self = static_cast<ActivityPlacer*>(self_);
		self->rotateHover(MOVEABLE::ROT::CLOCKWISE);
		return std::make_pair(CONTINUATION::CONTINUE, std::nullopt);
	});
}

void ActivityPlacer::appendRenderInfoInternal(GameState& gameState, RenderInfo& renderInfo) {
	ActivitySelector::appendRenderInfoInternal(gameState, renderInfo);
	if (hover.isNotNull()) {
		hover.get()->appendSelectionInfo(gameState, renderInfo);
	}
}

