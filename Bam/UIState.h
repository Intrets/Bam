#pragma once

#include "ReferenceManager.h"
#include "UIOBase.h"

struct State;

class UIState
{
private:
	glm::vec2 cursorScreen;
	glm::vec2 cursorWorld;

	CallBackBindResult runFrontBinds(State& state);

public:
	std::list<UniqueReference<UIOBase, UIOBase>> UIs;
	std::vector<UniqueReference<UIOBase,UIOBase>> UIsBuffer;

	glm::vec2 getCursorPositionWorld();
	glm::vec2 getCursorPositionScreen();
	glm::vec2 getCursorPositionScreenClamped(float c);
	
	void runUIBinds(State& state);
	void run(State& state);

	bool updateSize(GLFWwindow* window);
	void updateCursor(GLFWwindow* window, glm::vec2 cam);
	void appendRenderInfo(GameState& gameState, RenderInfo& renderInfo);

	void addUI(UniqueReference<UIOBase, UIOBase> ref);

	UIState();
	~UIState() = default;
};

