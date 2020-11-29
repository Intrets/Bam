#pragma once

#include "UIOBase.h"

class UIODestructible;
class UIState;

class UIOAnchoredProxy : public UIOBaseEnd
{
public:
	ManagedReference<UIOBase, UIOBase> proxyBase;
	UIODestructible* destructible;

	void setProxy(UniqueReference<UIOBase, UIOBase> ref, UIState& uiState);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) override;

	UIOAnchoredProxy(Handle self);
	virtual ~UIOAnchoredProxy();
};

