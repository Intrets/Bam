#pragma once

#include "GameState.h"
#include "UIState.h"
#include "Player.h"

class ControlState;

struct State
{
	GameState gameState;
	// Has to live in global for letting glfw handle key input and mouse input
	ControlState& controlState;
	UIState uiState;
	Player player;

	State(ControlState& c) : controlState(c) {
	};
};

