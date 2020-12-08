#pragma once

#include "UIOBase.h"
#include "Enums.h"

class UIODestructible;
class UIState;

class UIOAnchoredProxy : public UIOBaseEnd
{
private:
	UIO::ALIGNMENT alignment = UIO::ALIGNMENT::TOP;

public:
	ManagedReference<UIOBase, UIOBase> proxyBase;
	UIODestructible* destructible;

	void closeProxy();
	bool hasProxy();
	void setProxy(UniqueReference<UIOBase, UIOBase> ref, UIState& uiState);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) override;

	UIOAnchoredProxy(Handle self);
	virtual ~UIOAnchoredProxy();
};

