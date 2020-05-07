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
	PLACEBLOCK_HOVER,
	TEST_UP,
	TEST_DOWN,
	TEST_LEFT,
	TEST_RIGHT,
	TEST_RETRACT,
	TEST_EXTEND,
	SELECTACTIVITY,
	SELECTACTIVITYROOT,
	TEST_CYCLEBLOCK,
	TEST_SAVE,
	TEST_LOAD,
	TEST_EXIT,
	TOGGLE_DEBUG,
	MOUSE_POS_CHANGED,
	CANCEL,
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

	void key_callback(GLFWwindow* w, int key, int scancode, int action, int mods);
private:
	std::optional<std::vector<BindControl>> cachedBindControls;

private:
	friend class BindHandler;
	std::array<CONTROLS, GLFW_KEY_LAST + GLFW_MOUSE_BUTTON_LAST> keyToControl;
	std::array<CONTROLSTATE, CONTROLS::CONTROLS_MAX> controlState;
};

