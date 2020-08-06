#include "common.h"

#include "UIOActivitySelector.h"
#include "GameState.h"
#include "Platform.h"
#include "UIOCallBackParams.h"

UIOActivitySelector::UIOActivitySelector(Handle self) {
	this->selfHandle = self;

	this->addGlobalBind({ CONTROLS::ACTION5, CONTROLSTATE::CONTROLSTATE_PRESSED }, [&](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult {
		params.player.selection.expandTarget();
		return BIND_RESULT::CONTINUE;
	});

	this->addGlobalBind({ CONTROLS::ACTION6, CONTROLSTATE::CONTROLSTATE_PRESSED }, [&](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult {
		params.player.selection.shrinkTarget();
		return BIND_RESULT::CONTINUE;
	});

	this->addGlobalBind({ CONTROLS::ACTION0, CONTROLSTATE::CONTROLSTATE_PRESSED }, [&](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult {
		params.player.selection.selectTarget(params.gameState, params.uiState.getCursorPositionWorld());
		return BIND_RESULT::CONTINUE;
	});

	this->addGlobalBind({ CONTROLS::MOUSE_POS_CHANGED, CONTROLSTATE::CONTROLSTATE_PRESSED }, [&](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult {
		auto& selection = params.player.selection;
		if (selection.type == SELECTION_TYPE::HOVERING) {
			if (selection.target.isValid()) {
				auto pos = glm::ivec2(glm::floor(params.uiState.getCursorPositionWorld()));
				selection.target.get()->forceMoveOriginUp(pos - selection.target.get()->getOrigin());
			}
		}
		return BIND_RESULT::CONTINUE;
	});

	this->addGlobalBind({ CONTROLS::ROTATEL, CONTROLSTATE::CONTROLSTATE_PRESSED | CONTROLSTATE::CONTROLSTATE_REPEAT }, [&](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult {
		params.player.selection.rotateHover(MOVEABLE::COUNTERCLOCKWISE);
		return BIND_RESULT::CONTINUE;
	});

	this->addGlobalBind({ CONTROLS::ROTATER, CONTROLSTATE::CONTROLSTATE_PRESSED | CONTROLSTATE::CONTROLSTATE_REPEAT }, [&](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult {
		params.player.selection.rotateHover(MOVEABLE::COUNTERCLOCKWISE);
		return BIND_RESULT::CONTINUE;
	});
}

ScreenRectangle UIOActivitySelector::updateSize(ScreenRectangle newScreenRectangle) {
	return newScreenRectangle;
}

int32_t UIOActivitySelector::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	return depth;
}
