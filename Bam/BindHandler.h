#pragma once

#include "ControlState.h"
#include <functional>

#include "UIOBase.h"

class GameState;
class LogicSequencer;
struct RenderInfo;

class BindHandler
{
public:
	void appendRenderInfo(GameState& gameState, RenderInfo& renderInfo);
	void runBinds(ControlState& controlState, GameState& gameState);

	void addBind(CONTROLS control, CONTROLSTATE state, std::function<void(GameState&)> f);

	BindHandler();
	~BindHandler();

private:
	std::vector<std::unique_ptr<LogicSequencer>> logicSequences;

	UniqueReference<UIOBase, UIOBase> UI;

	ManagedReference<UIOBase, UIOBase> focus;

	using CallbackVector = std::vector<std::function<void(GameState&)>>;
	std::array<std::array<CallbackVector, CONTROLSTATE::CONTROLSTATE_MAX>, CONTROLS::CONTROLS_MAX > binds;
};

