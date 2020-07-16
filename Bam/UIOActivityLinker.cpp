#include "common.h"

#include "UIOActivityLinker.h"

#include "UIOCallBackParams.h"
#include "GameState.h"
#include "Linker.h"

UIOActivityLinker::UIOActivityLinker(Handle self) {
	this->selfHandle = self;

	addFocussedBind({ CONTROLS::ACTION0, CONTROLSTATE::CONTROLSTATE_PRESSED },
					[&](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult {
		if (!params.player.selection.target.isValid()) {
			return BIND_RESULT::CONTINUE;
		}
		auto cursor = params.gameState.getPlayerCursorWorldSpace();
		auto maybeTarget = params.gameState.staticWorld.getActivity(glm::floor(cursor));
		if (maybeTarget) {
			auto& target = maybeTarget.value();
			std::cout << Linker::link(params.gameState, params.player.selection.target, target) << "\n";
		}
		return BIND_RESULT::CONTINUE;
	}
	);
}

ScreenRectangle UIOActivityLinker::updateSize(ScreenRectangle newScreenRectangle) {
	return ScreenRectangle();
}
