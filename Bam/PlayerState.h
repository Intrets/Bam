#pragma once

#include "GameState.h"
#include "UIState.h"
#include "Player.h"
#include "NetworkAction.h"

class ControlState;

struct PlayerState
{
	Player* player;
	GameState& gameState;
	ControlState& controlState;
	UIState& uiState;

	PlayerActions playerActions;
	std::optional<std::unique_ptr<GameLoad>> maybeGameLoad;

	Player& getPlayer();
};

