#pragma once

#include "ReferenceManager.h"
#include "UIOBase.h"

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
	bool updateSize(GLFWwindow* window);
	void updateCursor(GLFWwindow* window, glm::vec2 cam);
	void appendRenderInfo(GameState& gameState, RenderInfo& renderInfo);

	UIState();
	~UIState() = default;
};

