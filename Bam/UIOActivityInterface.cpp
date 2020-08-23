#include "common.h"

#include "UIOActivityInterface.h"

#include "Colors.h"
#include "GameState.h"

UIOActivityInterface::UIOActivityInterface(Handle self) {
	this->selfHandle = self;
}

void UIOActivityInterface::setBase(WeakReference<Activity, Activity> ref) {
	this->base.set(ref);
	this->baseSelectionTick = 0;
}

void UIOActivityInterface::setTarget(WeakReference<Activity, Activity> ref) {
	this->target.set(ref);
	this->targetSelectionTick = 0;
}

int32_t UIOActivityInterface::addRenderInfo(GameState const& gameState, RenderInfo& renderInfo, int32_t depth) {
	if (this->base.isValid()) {
		if (this->baseSelectionTick == 0) {
			this->baseSelectionTick = gameState.tick;
		}
		if (periodic(gameState.tick, 80, 40, -this->baseSelectionTick)) {
			this->base.get()->appendSelectionInfo(gameState, renderInfo, COLORS::GR::SELECTION);
		}
	}
	if (this->target.isValid()) {
		if (this->targetSelectionTick == 0) {
			this->targetSelectionTick = gameState.tick;
		}
		if (periodic(gameState.tick, 40, 20, -this->targetSelectionTick)) {
			this->target.get()->appendSelectionInfo(gameState, renderInfo, COLORS::GR::HIGHLIGHT);
		}
	}
	return depth;
}

ScreenRectangle UIOActivityInterface::updateSize(ScreenRectangle newScreenRectangle) {
	newScreenRectangle.setHeight(0.0f);
	newScreenRectangle.setWidth(0.0f);

	this->screenRectangle = newScreenRectangle;

	return this->screenRectangle;
}
