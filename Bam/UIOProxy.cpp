#include "common.h"

#include "UIOProxy.h"

ScreenRectangle UIOProxy::updateSize(ScreenRectangle newScreenRectangle) {
    return ScreenRectangle();
}

UIOProxy::UIOProxy(Handle self, WeakReference<UIOBase,UIOBase> m) {
    this->selfHandle = self;
    this->main.set(m);
}

CallBackBindResult UIOProxy::runGlobalBinds(State& state) {
    if (main.isValid()) {
        return main.get()->runGlobalBinds(state);
    }
    return BIND_RESULT::CONTINUE;
}

CallBackBindResult UIOProxy::runFocussedBinds(State& state) {
    if (main.isValid()) {
        return main.get()->runFocussedBinds(state);
    }
    return BIND_RESULT::CONTINUE;
}

int32_t UIOProxy::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
    if (main.isValid()) {
        return main.get()->addRenderInfo(gameState, renderInfo, depth);
    };
    return depth;
}
