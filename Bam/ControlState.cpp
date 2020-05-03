#include "common.h"
#include "ControlState.h"

#include <array>

ControlState::ControlState() {
	controlState.fill(CONTROLSTATE::CONTROLSTATE_UP);

	keyToControl[GLFW_KEY_A] = CONTROLS::LEFT;
	keyToControl[GLFW_KEY_D] = CONTROLS::RIGHT;
	keyToControl[GLFW_KEY_W] = CONTROLS::UP;
	keyToControl[GLFW_KEY_S] = CONTROLS::DOWN;
	keyToControl[GLFW_KEY_Q] = CONTROLS::ROTATEL;
	keyToControl[GLFW_KEY_E] = CONTROLS::ROTATER;
	keyToControl[GLFW_KEY_LAST + GLFW_MOUSE_BUTTON_1] = CONTROLS::PLACEBLOCK_HOVER;
	keyToControl[GLFW_KEY_LAST + GLFW_MOUSE_BUTTON_2] = CONTROLS::CANCEL;
	keyToControl[GLFW_KEY_LAST + GLFW_MOUSE_BUTTON_3] = CONTROLS::SELECTACTIVITY;
	keyToControl[GLFW_KEY_LEFT] = CONTROLS::TEST_LEFT;
	keyToControl[GLFW_KEY_RIGHT] = CONTROLS::TEST_RIGHT;
	keyToControl[GLFW_KEY_DOWN] = CONTROLS::TEST_DOWN;
	keyToControl[GLFW_KEY_UP] = CONTROLS::TEST_UP;
	keyToControl[GLFW_KEY_Z] = CONTROLS::TEST_EXTEND;
	keyToControl[GLFW_KEY_X] = CONTROLS::TEST_RETRACT;
	keyToControl[GLFW_KEY_LAST + GLFW_MOUSE_BUTTON_4] = CONTROLS::SELECTACTIVITYROOT;
}

void ControlState::cycleStates() {
	for (auto& state : controlState) {
		if (state == CONTROLSTATE::CONTROLSTATE_PRESSED) {
			state = CONTROLSTATE::CONTROLSTATE_DOWN;
		}
		else if (state == CONTROLSTATE::CONTROLSTATE_RELEASED) {
			state = CONTROLSTATE::CONTROLSTATE_UP;
		}
	}
}

void ControlState::key_callback(GLFWwindow* w, int key, int scancode, int action, int mods) {
	// unknown key?
	if (key == -1) {
		return;
	}

	auto state = CONTROLSTATE::CONTROLSTATE_PRESSED;
	if (action == GLFW_RELEASE) {
		state = CONTROLSTATE::CONTROLSTATE_DOWN;
	}

	controlState[keyToControl[key]] = state;
}
