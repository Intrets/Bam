#pragma once

#include "ControlState.h"
#include <functional>

class GameState;

class BindHandler
{
public:
	void runBinds(ControlState& controlState, GameState& gameState);

	void addBind(CONTROLS control, CONTROLSTATE state, std::function<void(GameState&)> f);

	BindHandler();
	~BindHandler();

private:
	//std::array<std::vector<std::function<void(GameState&)>>, 2 * CONTROLS::CONTROLS_MAX> binds;
	using CallbackVector = std::vector<std::function<void(GameState&)>>;
	std::array<std::array<CallbackVector, CONTROLSTATE::CONTROLSTATE_MAX>, CONTROLS::CONTROLS_MAX > binds;
};

