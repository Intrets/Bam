#pragma once

#include "ControlState.h"
#include <functional>

#include "UIOBase.h"
#include <deque>

class GameState;
class LogicSequencer;
struct RenderInfo;

class BindHandler
{
public:
	void updateWindowSize(GLFWwindow* window);
	void appendRenderInfo(GameState& gameState, RenderInfo& renderInfo);
	void runBinds(ControlState& controlState, GameState& gameState);

	BindHandler(GLFWwindow* window);
	~BindHandler();

private:
	std::vector<std::unique_ptr<LogicSequencer>> logicSequences;

	//UniqueReference<UIOBase, UIOBase> UI;
	std::deque<UniqueReference<UIOBase, UIOBase>> UIs;
	//std::vector<UniqueReference<UIOBase, UIOBase>> UIs;
	//ManagedReference<UIOBase, UIOBase> focus;

};

