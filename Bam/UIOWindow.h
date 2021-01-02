#pragma once

#include "UIOBase.h"
#include "UIOButton.h"

class UIOWindow : public UIOBaseMulti
{
public:
	enum TYPE
	{
		MINIMISE = 1 << 0,
		CLOSE = 1 << 1,
		RESIZE = 1 << 2,
		RESIZEHORIZONTAL = 1 << 3,
		RESIZEVERTICAL = 1 << 4,
		MOVE = 1 << 5,
		HIDE = 1 << 6,
	};

	WeakReference<UIOBase, UIOBase> main;
	WeakReference<UIOBase, UIOBase> topBar;

	bool minimized = false;

	UIOWindow(Handle self);
	virtual ~UIOWindow() = default;

	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) override;

	virtual CallBackBindResult runOnHoverBinds(PlayerState& playerState) override;
	virtual CallBackBindResult runFocussedBinds(PlayerState& playerState) override;
	virtual CallBackBindResult runActiveBinds(PlayerState& playerState) override;
	virtual CallBackBindResult runGlobalBinds(PlayerState& playerState) override;
	virtual CallBackBindResult runGameWorldBinds(PlayerState& playerState) override;

	virtual void addElement(UniqueReference<UIOBase, UIOBase> element) override;
	virtual void addElementMulti(UniqueReference<UIOBase, UIOBase> element);
};

