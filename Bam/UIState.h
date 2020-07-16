#pragma once

#include "UIOActivitySelector.h"

struct State;

class UIState
{
public:
	std::list<UniqueReference<UIOBase, UIOBase>> UIs;
	
	void run(State& state);
	void updateSize(GLFWwindow* window);
	void appendRenderInfo(GameState& gameState, RenderInfo& renderInfo);

	UIState();
	~UIState() = default;
};

