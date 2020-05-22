#pragma once

#include <array>
#include <optional>

typedef enum
{
	NOTHING,
	UP,
	DOWN,
	LEFT,
	RIGHT,
	JUMP,
	ZOOM_IN,
	ZOOM_OUT,
	ROTATEL,
	ROTATER,
	DESTROYBLOCK,
	PLACEBLOCK,
	ACTION1,
	TEST_UP,
	TEST_DOWN,
	TEST_LEFT,
	TEST_RIGHT,
	TEST_RETRACT,
	TEST_EXTEND,
	ACTION0,
	ACTION3,
	ACTION4,
	ACTION5,
	ACTION6,
	SELECTACTIVITYROOT,
	TEST_CYCLEBLOCK,
	TEST_SAVE,
	TEST_LOAD,
	TEST_EXIT,
	TOGGLE_DEBUG,
	MOUSE_POS_CHANGED,
	ACTION2,
	TOOL_1,
	TOOL_2,
	TOOL_3,
	TOOL_4,
	CONTROLS_MAX
} CONTROLS;

typedef enum
{
	CONTROLSTATE_RELEASED,
	CONTROLSTATE_PRESSED,
	CONTROLSTATE_DOWN,
	CONTROLSTATE_UP,
	CONTROLSTATE_MAX
} CONTROLSTATE;

typedef enum
{
	CONTROLSTATE_RELEASED_MASK = 1 << 0,
	CONTROLSTATE_PRESSED_MASK = 1 << 1,
	CONTROLSTATE_DOWN_MASK = 1 << 2,
	CONTROLSTATE_UP_MASK = 1 << 3,
} CONTROLSTATE_MASK;

typedef enum
{
	CONSUMED_NOTHING = 0,
	MOUSEPRESS = 1 << 0,
	KEYPRESS = 1 << 1,
} CONSUMED;

struct BindControl
{
	CONTROLS control;
	CONTROLSTATE state;

	bool operator== (const BindControl& other) const {
		return this->control == other.control && this->state == other.state;
	}
};

struct BindControlHash
{
	std::size_t operator() (const BindControl& c) const {
		return c.control + (c.state >> 16);
	}
};

class ControlState
{
public:
	ControlState();

	void cycleStates();
	std::vector<BindControl> getBindControls();

	std::string getCharBuffer();

	bool activated(BindControl bindControl);

	void key_callback(GLFWwindow* w, int32_t key, int32_t scancode, int32_t action, int32_t mods);
	void char_callback(GLFWwindow* window, unsigned int character);
private:
	std::optional<std::vector<BindControl>> cachedBindControls;

private:
	friend class BindHandler;
	std::array<CONTROLS, GLFW_KEY_LAST + GLFW_MOUSE_BUTTON_LAST> keyToControl;
	std::array<CONTROLSTATE, CONTROLS::CONTROLS_MAX> controlState;
	std::string charBuffer;
};

