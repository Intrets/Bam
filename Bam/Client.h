#pragma once

#include "Renderer.h"
#include "UIState.h"
#include "RenderLimiter.h"
#include "PlayerState.h"
#include "ControlState.h"

struct Client
{
	Renderer renderer;
	UIState uiState;
	RenderLimiter renderLimiter;
	PlayerState state;

	Client(GameState& gameState, ControlState& controlState) :
		state({ gameState.getPlayer(0).value(), gameState, controlState, uiState }) {
	};
};

