#pragma once

#include "ControlState.h"
#include <functional>

class GameState;
class LogicSequencer;

class BindHandler
{
public:
	void runBinds(ControlState& controlState, GameState& gameState);

	void addBind(CONTROLS control, CONTROLSTATE state, std::function<void(GameState&)> f);

	BindHandler();
	~BindHandler();

private:
	std::vector<std::unique_ptr<LogicSequencer>> logicSequences;

	using CallbackVector = std::vector<std::function<void(GameState&)>>;
	std::array<std::array<CallbackVector, CONTROLSTATE::CONTROLSTATE_MAX>, CONTROLS::CONTROLS_MAX > binds;
};

