#include "common.h"

#include "UIOActivityLinker.h"

#include "GameState.h"
#include "Linker.h"

static CallBackBindResult link(UIOCallBackParams& state, UIOBase* self_) {
	return BIND_RESULT::CONTINUE;
}  

UIOActivityLinker::UIOActivityLinker(Handle self) {
	this->selfHandle = self;

	addFocussedBind({ CONTROLS::ACTION0, CONTROLSTATE::CONTROLSTATE_PRESSED }, link);
}

ScreenRectangle UIOActivityLinker::updateSize(ScreenRectangle newScreenRectangle) {
	return ScreenRectangle();
}
