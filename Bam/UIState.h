#pragma once

#include "ReferenceManager.h"
#include "UIOBase.h"

#include <GLM/glm.hpp>
#include <list>
#include <vector>
#include <unordered_map>
#include <string>

struct PlayerState;

class UIState
{
private:
	bool shouldReset_ = false;

	glm::vec2 cursorScreen;
	glm::vec2 cursorWorld;

	CallBackBindResult runFrontBinds(PlayerState& playerState);

	std::unordered_map<std::string, ManagedReference<UIOBase, UIOBase>> namedUIs;

public:
	std::list<UniqueReference<UIOBase, UIOBase>> UIs;

	std::vector<UniqueReference<UIOBase, UIOBase>> UIsBuffer;
	std::unordered_map<std::string, UniqueReference<UIOBase, UIOBase>> namedUIsBuffer;

	std::vector<UniqueReference<UIOBase, UIOBase>> closedBuffer;

	glm::vec2 getCursorPositionWorld();
	glm::vec2 getCursorPositionScreen();
	glm::vec2 getCursorPositionScreenClamped(float c);

	void runUIBinds(PlayerState& playerState);
	void run(PlayerState& playerState);

	bool updateSize(GLFWwindow* window);
	void updateCursor(GLFWwindow* window, glm::vec2 cam);
	void appendRenderInfo(GameState& gameState, RenderInfo& renderInfo);

	void addUI(UniqueReference<UIOBase, UIOBase> ref);

	// Returns if new UI is created or previous one is brought to the front
	// true - new object created
	// false - already exists and brought to the front
	bool addNamedUI(std::string const& name, std::function<UniqueReference<UIOBase, UIOBase>()> f);

	void addNamedUIReplace(std::string const& name, UniqueReference<UIOBase, UIOBase> ref);

	void closeNamedUI(std::string const& name);
	void closeUI(WeakReference<UIOBase, UIOBase> ref);

	void reset();
	bool shouldReset();

	void init();
	void clear();

	UIState();
	~UIState() = default;

	NOCOPYMOVE(UIState);
};

