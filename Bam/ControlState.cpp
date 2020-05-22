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
	keyToControl[GLFW_KEY_LAST + GLFW_MOUSE_BUTTON_1] = CONTROLS::ACTION0;
	keyToControl[GLFW_KEY_LAST + GLFW_MOUSE_BUTTON_2] = CONTROLS::ACTION1;
	keyToControl[GLFW_KEY_LAST + GLFW_MOUSE_BUTTON_3] = CONTROLS::ACTION2;
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
	keyToControl[GLFW_KEY_1] = CONTROLS::TOOL_1;
	keyToControl[GLFW_KEY_2] = CONTROLS::TOOL_2;
	keyToControl[GLFW_KEY_3] = CONTROLS::TOOL_3;
	keyToControl[GLFW_KEY_4] = CONTROLS::TOOL_4;
	keyToControl[GLFW_KEY_Z] = CONTROLS::ACTION3;
	keyToControl[GLFW_KEY_X] = CONTROLS::ACTION4;
	keyToControl[GLFW_KEY_C] = CONTROLS::ACTION5;
	keyToControl[GLFW_KEY_V] = CONTROLS::ACTION6;

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
	charBuffer.clear();
}

std::vector<BindControl> ControlState::getBindControls() {
	if (!cachedBindControls.has_value()) {
		cachedBindControls.emplace();
		for (int32_t i = 0; i < CONTROLS::CONTROLS_MAX; i++) {
			CONTROLS control = static_cast<CONTROLS>(i);
			CONTROLSTATE state = controlState[i];
			cachedBindControls->push_back({ control, state });
		}
	}
	return cachedBindControls.value();
}

std::string ControlState::getCharBuffer() {
	return charBuffer;
}

bool ControlState::activated(BindControl bindControl) {
	return controlState[bindControl.control] == bindControl.state;
}

void ControlState::key_callback(GLFWwindow* w, int32_t key, int32_t scancode, int32_t action, int32_t mods) {
	// unknown key?
	if (key == -1) {
		return;
	}

	if (key == GLFW_KEY_ENTER && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		charBuffer.push_back('-');
		charBuffer.push_back('\n');
	}

	if (action == GLFW_RELEASE) {
		controlState[keyToControl[key]] = CONTROLSTATE::CONTROLSTATE_RELEASED;
	}
	else if (action == GLFW_PRESS) {
		controlState[keyToControl[key]] = CONTROLSTATE::CONTROLSTATE_PRESSED;
	}

}

void ControlState::char_callback(GLFWwindow* window, unsigned int character) {
	if (character > 127) {
		return;
	}
	char c = static_cast<char>(character);
	charBuffer.push_back(c);
}
