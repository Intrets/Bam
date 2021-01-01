#pragma once

#include "GameState.h"
#include "UIState.h"
#include "Player.h"

class ControlState;

struct PlayerState
{
	Player* player;
	GameState& gameState;
	ControlState& controlState;
	UIState& uiState;

	Player& getPlayer();
};

