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

	BindHandler();
	~BindHandler();

private:
	std::vector<std::unique_ptr<LogicSequencer>> logicSequences;

	//UniqueReference<UIOBase, UIOBase> UI;
	std::vector<UniqueReference<UIOBase, UIOBase>> UIs;

};

