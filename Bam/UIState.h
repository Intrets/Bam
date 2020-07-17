#pragma once

#include "UIOActivitySelector.h"

struct State;

class UIState
{
private:
	glm::vec2 cursorScreen;
	glm::vec2 cursorWorld;

public:
	std::list<UniqueReference<UIOBase, UIOBase>> UIs;

	glm::vec2 getCursorPositionWorld();
	glm::vec2 getCursorPositionScreen();
	
	void run(State& state);
	void updateSize(GLFWwindow* window);
	void updateCursor(GLFWwindow* window, glm::vec2 cam);
	void appendRenderInfo(GameState& gameState, RenderInfo& renderInfo);

	UIState();
	~UIState() = default;
};

