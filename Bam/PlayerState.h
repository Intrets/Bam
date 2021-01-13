#pragma once

#include "GameState.h"
#include "UIState.h"
#include "Player.h"
#include "PlayerActions.h"
#include "MetaActions.h"

class ControlState;

struct PlayerState
{
	Player* player;
	GameState& gameState;
	ControlState& controlState;
	UIState& uiState;

	PlayerActions playerActions;
	MetaActions metaActions;

	Player& getPlayer();
};

