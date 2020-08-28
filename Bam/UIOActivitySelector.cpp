#include "common.h"

#include "UIOActivitySelector.h"
#include "GameState.h"
#include "Platform.h"
#include "UIOCallBackParams.h"

UIOActivitySelector::UIOActivitySelector(Handle self) {
	this->selfHandle = self;

	this->addGlobalBind({ ControlState::CONTROLS::ACTION5, ControlState::CONTROLSTATE_PRESSED }, [&](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult {
		params.player.selection.expandTarget();
		return BIND_RESULT::CONTINUE;
	});

	this->addGlobalBind({ ControlState::CONTROLS::ACTION6, ControlState::CONTROLSTATE_PRESSED }, [&](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult {
		params.player.selection.shrinkTarget();
		return BIND_RESULT::CONTINUE;
	});

	this->addGlobalBind({ ControlState::CONTROLS::ACTION0, ControlState::CONTROLSTATE_PRESSED }, [&](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult {
		params.player.selection.selectTarget(params.gameState, params.uiState.getCursorPositionWorld());
		return BIND_RESULT::CONTINUE;
	});

	this->addGlobalBind({ ControlState::CONTROLS::MOUSE_POS_CHANGED, ControlState::CONTROLSTATE_PRESSED }, [&](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult {
		auto& selection = params.player.selection;
		if (selection.type == SELECTION_TYPE::HOVERING) {
			if (selection.target.isValid()) {
				auto pos = glm::ivec2(glm::floor(params.uiState.getCursorPositionWorld()));
				selection.target.get()->forceMoveOriginUp(pos - selection.target.get()->getOrigin());
			}
		}
		return BIND_RESULT::CONTINUE;
	});

	this->addGlobalBind({ ControlState::CONTROLS::ROTATEL, ControlState::CONTROLSTATE_PRESSED | ControlState::CONTROLSTATE_REPEAT }, [&](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult {
		params.player.selection.rotateHover(Activity::ROT::COUNTERCLOCKWISE);
		return BIND_RESULT::CONTINUE;
	});

	this->addGlobalBind({ ControlState::CONTROLS::ROTATER, ControlState::CONTROLSTATE_PRESSED | ControlState::CONTROLSTATE_REPEAT }, [&](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult {
		params.player.selection.rotateHover(Activity::ROT::COUNTERCLOCKWISE);
		return BIND_RESULT::CONTINUE;
	});
}

ScreenRectangle UIOActivitySelector::updateSize(ScreenRectangle newScreenRectangle) {
	return newScreenRectangle;
}

int32_t UIOActivitySelector::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	return depth;
}
