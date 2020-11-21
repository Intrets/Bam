#include "common.h"

#include "UIOLua.h"

void UIOLua::setWatched(WeakReference<Activity, LuaActivity> ref) {
	this->watched.set(ref);
}

ManagedReference<Activity, LuaActivity> const& UIOLua::getWatched() {
	return watched;
}

UIOLua::UIOLua(Handle self, WeakReference<Activity, LuaActivity> ref) {
	this->selfHandle = self;
	this->setWatched(ref);
}

ScreenRectangle UIOLua::updateSize(ScreenRectangle newScreenRectangle) {
	newScreenRectangle.setHeight(0.0f);
	newScreenRectangle.setWidth(0.0f);

	this->screenRectangle = newScreenRectangle;

	return this->screenRectangle;
}

int32_t UIOLua::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	return depth;
}
