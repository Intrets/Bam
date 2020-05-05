#pragma once

#include <array>

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

class ControlState
{
public:
	ControlState();

	void cycleStates();

	void key_callback(GLFWwindow* w, int key, int scancode, int action, int mods);
private:
	friend class BindHandler;
	std::array<CONTROLS, GLFW_KEY_LAST + GLFW_MOUSE_BUTTON_LAST> keyToControl;
	std::array<CONTROLSTATE, CONTROLS::CONTROLS_MAX> controlState;
};

