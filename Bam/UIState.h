#pragma once

#include "UIOActivitySelector.h"

class UIState
{
public:
	WeakReference<UIOBase, UIOActivitySelector> selector;

	std::list<UniqueReference<UIOBase, UIOBase>> UIs;
	
	void run(GameState& gameState, ControlState& controlState);
	void updateSize(GLFWwindow* window);
	void appendRenderInfo(GameState& gameState, RenderInfo& renderInfo);

	UIState();
	~UIState() = default;
};

