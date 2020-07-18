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
	TOOL_0,
	TOOL_1,
	TOOL_2,
	TOOL_3,
	TOOL_4,
	TOOL_5,
	TOOL_6,
	TOOL_7,
	TOOL_8,
	TOOL_9,
	SCROLL_UP,
	SCROLL_DOWN,
	BACKSPACE,
	DELETE,
	TAB,
	CHAR_BUFFER_CHANGED,
	EVERY_TICK,
	CONTROLS_MAX,
} CONTROLS;

typedef enum
{
	CONTROLSTATE_UP = 0,
	CONTROLSTATE_RELEASED = 1 << 0,
	CONTROLSTATE_PRESSED = 1 << 1,
	CONTROLSTATE_DOWN = 1 << 2,
	CONTROLSTATE_REPEAT = 1 << 3,
} CONTROLSTATE;

struct BindControl
{
	CONTROLS control;
	int32_t state;

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

	std::string getCharBuffer();
	int32_t scrollDistance;

	void consumeControl(CONTROLS control);

	bool activated(BindControl bindControl);

	void key_callback(GLFWwindow* w, int32_t key, int32_t scancode, int32_t action, int32_t mods);
	void char_callback(GLFWwindow* window, unsigned int character);
	void scroll_callback(GLFWwindow* w, double xoffset, double yoffset);

private:
	std::array<CONTROLS, GLFW_KEY_LAST + GLFW_MOUSE_BUTTON_LAST> keyToControl;
	std::array<int32_t, CONTROLS::CONTROLS_MAX> controlState;
	std::array<bool, CONTROLS::CONTROLS_MAX> consumed;
	std::string charBuffer;
};

