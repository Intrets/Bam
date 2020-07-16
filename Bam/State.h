#pragma once

#include "GameState.h"
#include "UIState.h"
#include "Player.h"

class ControlState;

struct State
{
	GameState gameState;
	ControlState& controlState;
	UIState uiState;
	Player player;

	State(ControlState& c) : controlState(c) {
	};
};

