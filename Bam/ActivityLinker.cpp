#include "common.h"

#include "ActivityLinker.h"
#include "GameState.h"

#include "Linker.h"

ActivityLinker::ActivityLinker() {
	addBind({ CONTROLS::ACTION0, CONTROLSTATE::CONTROLSTATE_PRESSED }, [](GameState& gameState, LogicSequencer* self_) {
		auto self = static_cast<ActivityLinker*>(self_);
		auto maybeTarget = gameState.staticWorld.getActivity(gameState.getPlayerCursorWorldSpace());

		if (!maybeTarget.has_value()) {
			return std::make_pair(CONTINUATION::STOP, std::nullopt);
		}

		if (!self->target) {
			self->target = maybeTarget.value();
		}
		else {
			if (self->target.get()->getType() == ACTIVITY::PISTON) {
				Linker::linkPiston(gameState, self->target, maybeTarget.value());
				self->target.clear();
			}
			else {
				Linker::link(gameState, maybeTarget.value(), self->target);
				self->target.clear();
			}
		}

		return std::make_pair(CONTINUATION::STOP, std::nullopt);
	});
}

void ActivityLinker::appendRenderInfoInternal(GameState& gameState, RenderInfo& renderInfo) {
	if (target) {
		target.get()->appendSelectionInfo(gameState, renderInfo);
	}
}
