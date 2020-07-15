#pragma once

#include "GameState.h"
#include "UIState.h"

class ControlState;

struct State
{
	GameState gameState;
	ControlState& controlState;
	UIState uiState;

	State(ControlState& c) : controlState(c) {
	};
};

