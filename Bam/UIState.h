#pragma once

#include "ReferenceManager.h"
#include "UIOBase.h"

struct State;

class UIState
{
private:
	bool shouldReset_ = false;

	glm::vec2 cursorScreen;
	glm::vec2 cursorWorld;

	CallBackBindResult runFrontBinds(State& state);

	std::unordered_map<std::string, ManagedReference<UIOBase, UIOBase>> namedUIs;

public:
	std::list<UniqueReference<UIOBase, UIOBase>> UIs;

	std::vector<UniqueReference<UIOBase, UIOBase>> UIsBuffer;
	std::unordered_map<std::string, UniqueReference<UIOBase, UIOBase>> namedUIsBuffer;

	std::vector<UniqueReference<UIOBase, UIOBase>> closedBuffer;

	glm::vec2 getCursorPositionWorld();
	glm::vec2 getCursorPositionScreen();
	glm::vec2 getCursorPositionScreenClamped(float c);

	void runUIBinds(State& state);
	void run(State& state);

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
	void closeUI(Handle handle);

	void reset();
	bool shouldReset();

	void init();
	void clear();

	UIState();
	~UIState() = default;

	NOCOPYMOVE(UIState);
};

