#include "common.h"
#include "ControlState.h"

#include <array>
#include <iostream>

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
	keyToControl[GLFW_KEY_R] = CONTROLS::ZOOM_IN;
	keyToControl[GLFW_KEY_F] = CONTROLS::ZOOM_OUT;
	keyToControl[GLFW_KEY_F7] = CONTROLS::TEST_SAVE;
	keyToControl[GLFW_KEY_F8] = CONTROLS::TEST_LOAD;
	keyToControl[GLFW_KEY_F9] = CONTROLS::TOGGLE_DEBUG;
	keyToControl[GLFW_KEY_ESCAPE] = CONTROLS::TEST_EXIT;
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
	cachedBindControls.reset();
}

std::vector<BindControl> ControlState::getBindControls() {
	if (!cachedBindControls.has_value()) {
		cachedBindControls.emplace();
		for (int i = 0; i < CONTROLS::CONTROLS_MAX; i++) {
			CONTROLS control = static_cast<CONTROLS>(i);
			CONTROLSTATE state = controlState[i];
			cachedBindControls->push_back({ control, state });
		}
	}
	return cachedBindControls.value();
}

void ControlState::key_callback(GLFWwindow* w, int key, int scancode, int action, int mods) {
	// unknown key?
	if (key == -1) {
		return;
	}

	if (action == GLFW_RELEASE) {
		controlState[keyToControl[key]] = CONTROLSTATE::CONTROLSTATE_RELEASED;
	}
	else if (action == GLFW_PRESS) {
		controlState[keyToControl[key]] = CONTROLSTATE::CONTROLSTATE_PRESSED;
	}

}
