#include "common.h"

#include "UIOActivityLinker.h"

#include "UIOCallBackParams.h"
#include "GameState.h"
#include "Linker.h"

UIOActivityLinker::UIOActivityLinker(Handle self) {
	this->selfHandle = self;

	addGlobalBind({ ControlState::CONTROLS::ACTION0, ControlState::CONTROLSTATE_PRESSED },
				  [&](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		if (!params.player.selection.target.isValid()) {
			return BIND_RESULT::CONTINUE;
		}
		auto cursor = params.uiState.getCursorPositionWorld();
		auto maybeTarget = params.gameState.staticWorld.getActivity(glm::floor(cursor));
		if (maybeTarget) {
			auto& target = maybeTarget.value();
			Locator<Log>::ref().putStreamLine(std::stringstream() << Linker::link(params.gameState, params.player.selection.target, target));
			return BIND_RESULT::CONSUME;
		}
		return BIND_RESULT::CONTINUE;
	}
	);
}

ScreenRectangle UIOActivityLinker::updateSize(ScreenRectangle newScreenRectangle) {
	return ScreenRectangle();
}
