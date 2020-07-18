#include "common.h"
#include "ControlState.h"

#include <array>
#include <iostream>

ControlState::ControlState() {
	controlState.fill(0);

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

	keyToControl[GLFW_KEY_1] = CONTROLS::TOOL_0;
	keyToControl[GLFW_KEY_2] = CONTROLS::TOOL_1;
	keyToControl[GLFW_KEY_3] = CONTROLS::TOOL_2;
	keyToControl[GLFW_KEY_4] = CONTROLS::TOOL_3;
	keyToControl[GLFW_KEY_5] = CONTROLS::TOOL_4;
	keyToControl[GLFW_KEY_6] = CONTROLS::TOOL_5;
	keyToControl[GLFW_KEY_7] = CONTROLS::TOOL_6;
	keyToControl[GLFW_KEY_8] = CONTROLS::TOOL_7;
	keyToControl[GLFW_KEY_9] = CONTROLS::TOOL_8;
	keyToControl[GLFW_KEY_0] = CONTROLS::TOOL_9;

	keyToControl[GLFW_KEY_Z] = CONTROLS::ACTION3;
	keyToControl[GLFW_KEY_X] = CONTROLS::ACTION4;
	keyToControl[GLFW_KEY_C] = CONTROLS::ACTION5;
	keyToControl[GLFW_KEY_V] = CONTROLS::ACTION6;
	keyToControl[GLFW_KEY_BACKSPACE] = CONTROLS::BACKSPACE;
	keyToControl[GLFW_KEY_DELETE] = CONTROLS::DELETE;
	keyToControl[GLFW_KEY_TAB] = CONTROLS::TAB;
}

void ControlState::cycleStates() {
	for (auto& state : controlState) {
		state &= ~CONTROLSTATE::CONTROLSTATE_PRESSED;
		state &= ~CONTROLSTATE::CONTROLSTATE_RELEASED;
		state &= ~CONTROLSTATE::CONTROLSTATE_REPEAT;
	}
	charBuffer.clear();
	scrollDistance = 0;
	controlState[CONTROLS::MOUSE_POS_CHANGED] = CONTROLSTATE::CONTROLSTATE_PRESSED;
	controlState[CONTROLS::EVERY_TICK] = CONTROLSTATE::CONTROLSTATE_PRESSED;

	consumed.fill(false);
}

std::string ControlState::getCharBuffer() {
	return charBuffer;
}

void ControlState::consumeControl(CONTROLS control) {
	consumed[control] = true;
}

bool ControlState::activated(BindControl bindControl) {
	if (consumed[bindControl.control]) {
		return false;
	}
	else {
		return controlState[bindControl.control] & bindControl.state;
	}
}

void ControlState::key_callback(GLFWwindow* w, int32_t key, int32_t scancode, int32_t action, int32_t mods) {
	// unknown key?
	if (key == -1) {
		return;
	}

	if (key == GLFW_KEY_ENTER && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		charBuffer.push_back('\n');
		controlState[CHAR_BUFFER_CHANGED] = CONTROLSTATE::CONTROLSTATE_PRESSED;
	}

	switch (action) {
		case GLFW_REPEAT:
			controlState[keyToControl[key]] |= CONTROLSTATE::CONTROLSTATE_REPEAT;
			break;
		case GLFW_PRESS:
			controlState[keyToControl[key]] |= CONTROLSTATE::CONTROLSTATE_DOWN;
			controlState[keyToControl[key]] |= CONTROLSTATE::CONTROLSTATE_PRESSED;
			break;
		case GLFW_RELEASE:
			controlState[keyToControl[key]] &= ~CONTROLSTATE::CONTROLSTATE_DOWN;
			controlState[keyToControl[key]] |= CONTROLSTATE::CONTROLSTATE_RELEASED;
			break;
		default:
			break;
	}
}

void ControlState::char_callback(GLFWwindow* window, unsigned int character) {
	if (character > 127) {
		return;
	}
	controlState[CHAR_BUFFER_CHANGED] = CONTROLSTATE::CONTROLSTATE_PRESSED;
	char c = static_cast<char>(character);
	charBuffer.push_back(c);
}

void ControlState::scroll_callback(GLFWwindow* w, double xoffset, double yoffset) {
	if (yoffset < 0) {
		scrollDistance -= static_cast<int32_t>(floor(glm::abs(yoffset)));
	}
	else if (yoffset > 0) {
		scrollDistance += static_cast<int32_t>(floor(glm::abs(yoffset)));
	}

	if (scrollDistance < 0) {
		controlState[SCROLL_DOWN] = CONTROLSTATE::CONTROLSTATE_PRESSED;
		controlState[SCROLL_UP] = CONTROLSTATE::CONTROLSTATE_UP;
	}
	else if (scrollDistance > 0) {
		controlState[SCROLL_UP] = CONTROLSTATE::CONTROLSTATE_PRESSED;
		controlState[SCROLL_DOWN] = CONTROLSTATE::CONTROLSTATE_UP;
	}
	else {
		controlState[SCROLL_UP] = CONTROLSTATE::CONTROLSTATE_UP;
		controlState[SCROLL_DOWN] = CONTROLSTATE::CONTROLSTATE_UP;
	}
}
