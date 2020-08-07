#include "common.h"
#include "ControlState.h"

#include <array>
#include <iostream>

ControlState::ControlState() {
	this->controlState.fill(0);

	this->keyToControl[GLFW_KEY_A] = CONTROLS::LEFT;
	this->keyToControl[GLFW_KEY_D] = CONTROLS::RIGHT;
	this->keyToControl[GLFW_KEY_W] = CONTROLS::UP;
	this->keyToControl[GLFW_KEY_S] = CONTROLS::DOWN;
	this->keyToControl[GLFW_KEY_Q] = CONTROLS::ROTATEL;
	this->keyToControl[GLFW_KEY_E] = CONTROLS::ROTATER;
	this->keyToControl[GLFW_KEY_LAST + GLFW_MOUSE_BUTTON_1] = CONTROLS::ACTION0;
	this->keyToControl[GLFW_KEY_LAST + GLFW_MOUSE_BUTTON_2] = CONTROLS::ACTION1;
	this->keyToControl[GLFW_KEY_LAST + GLFW_MOUSE_BUTTON_3] = CONTROLS::ACTION2;
	this->keyToControl[GLFW_KEY_LEFT] = CONTROLS::TEST_LEFT;
	this->keyToControl[GLFW_KEY_RIGHT] = CONTROLS::TEST_RIGHT;
	this->keyToControl[GLFW_KEY_DOWN] = CONTROLS::TEST_DOWN;
	this->keyToControl[GLFW_KEY_UP] = CONTROLS::TEST_UP;
	this->keyToControl[GLFW_KEY_R] = CONTROLS::ZOOM_IN;
	this->keyToControl[GLFW_KEY_F] = CONTROLS::ZOOM_OUT;
	this->keyToControl[GLFW_KEY_F7] = CONTROLS::TEST_SAVE;
	this->keyToControl[GLFW_KEY_F8] = CONTROLS::TEST_LOAD;
	this->keyToControl[GLFW_KEY_F9] = CONTROLS::TOGGLE_DEBUG;
	this->keyToControl[GLFW_KEY_ESCAPE] = CONTROLS::TEST_EXIT;

	this->keyToControl[GLFW_KEY_1] = CONTROLS::TOOL_0;
	this->keyToControl[GLFW_KEY_2] = CONTROLS::TOOL_1;
	this->keyToControl[GLFW_KEY_3] = CONTROLS::TOOL_2;
	this->keyToControl[GLFW_KEY_4] = CONTROLS::TOOL_3;
	this->keyToControl[GLFW_KEY_5] = CONTROLS::TOOL_4;
	this->keyToControl[GLFW_KEY_6] = CONTROLS::TOOL_5;
	this->keyToControl[GLFW_KEY_7] = CONTROLS::TOOL_6;
	this->keyToControl[GLFW_KEY_8] = CONTROLS::TOOL_7;
	this->keyToControl[GLFW_KEY_9] = CONTROLS::TOOL_8;
	this->keyToControl[GLFW_KEY_0] = CONTROLS::TOOL_9;

	this->keyToControl[GLFW_KEY_Z] = CONTROLS::ACTION3;
	this->keyToControl[GLFW_KEY_X] = CONTROLS::ACTION4;
	this->keyToControl[GLFW_KEY_C] = CONTROLS::ACTION5;
	this->keyToControl[GLFW_KEY_V] = CONTROLS::ACTION6;
	this->keyToControl[GLFW_KEY_BACKSPACE] = CONTROLS::BACKSPACE;
	this->keyToControl[GLFW_KEY_DELETE] = CONTROLS::DELETE;
	this->keyToControl[GLFW_KEY_TAB] = CONTROLS::TAB;
}

void ControlState::cycleStates() {
	for (auto& state : this->controlState) {
		state &= ~ControlState::CONTROLSTATE_PRESSED;
		state &= ~ControlState::CONTROLSTATE_RELEASED;
		state &= ~ControlState::CONTROLSTATE_REPEAT;
	}
	this->charBuffer.clear();
	this->scrollDistance = 0;
	this->controlState[static_cast<int32_t>(CONTROLS::MOUSE_POS_CHANGED)] = ControlState::CONTROLSTATE_PRESSED;
	this->controlState[static_cast<int32_t>(CONTROLS::EVERY_TICK)] = ControlState::CONTROLSTATE_PRESSED;

	this->consumed.fill(false);
}

std::string ControlState::getCharBuffer() {
	return this->charBuffer;
}

void ControlState::consumeControl(CONTROLS control) {
	this->consumed[static_cast<size_t>(control)] = true;
}

bool ControlState::activated(BindControl bindControl) {
	if (this->consumed[static_cast<size_t>(bindControl.control)]) {
		return false;
	}
	else {
		return this->controlState[static_cast<size_t>(bindControl.control)] & bindControl.state;
	}
}

void ControlState::key_callback(GLFWwindow* w, int32_t key, int32_t scancode, int32_t action, int32_t mods) {
	// unknown key?
	if (key == -1) {
		return;
	}

	if (key == GLFW_KEY_ENTER && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		this->charBuffer.push_back('\n');
		this->controlState[static_cast<size_t>(CONTROLS::CHAR_BUFFER_CHANGED)] = ControlState::CONTROLSTATE_PRESSED;
	}

	switch (action) {
		case GLFW_REPEAT:
			this->controlState[static_cast<size_t>(keyToControl[key])] |= ControlState::CONTROLSTATE_REPEAT;
			break;
		case GLFW_PRESS:
			this->controlState[static_cast<size_t>(keyToControl[key])] |= ControlState::CONTROLSTATE_DOWN;
			this->controlState[static_cast<size_t>(keyToControl[key])] |= ControlState::CONTROLSTATE_PRESSED;
			break;
		case GLFW_RELEASE:
			this->controlState[static_cast<size_t>(keyToControl[key])] &= ~ControlState::CONTROLSTATE_DOWN;
			this->controlState[static_cast<size_t>(keyToControl[key])] |= ControlState::CONTROLSTATE_RELEASED;
			break;
		default:
			break;
	}
}

void ControlState::char_callback(GLFWwindow* window, unsigned int character) {
	if (character > 127) {
		return;
	}
	this->controlState[static_cast<size_t>(CONTROLS::CHAR_BUFFER_CHANGED)] = ControlState::CONTROLSTATE_PRESSED;
	char c = static_cast<char>(character);
	this->charBuffer.push_back(c);
}

void ControlState::scroll_callback(GLFWwindow* w, double xoffset, double yoffset) {
	if (yoffset < 0) {
		this->scrollDistance -= static_cast<int32_t>(floor(glm::abs(yoffset)));
	}
	else if (yoffset > 0) {
		this->scrollDistance += static_cast<int32_t>(floor(glm::abs(yoffset)));
	}

	if (scrollDistance < 0) {
		this->controlState[static_cast<size_t>(CONTROLS::SCROLL_DOWN)] = ControlState::CONTROLSTATE_PRESSED;
		this->controlState[static_cast<size_t>(CONTROLS::SCROLL_UP)] = ControlState::CONTROLSTATE_UP;
	}
	else if (scrollDistance > 0) {
		this->controlState[static_cast<size_t>(CONTROLS::SCROLL_UP)] = ControlState::CONTROLSTATE_PRESSED;
		this->controlState[static_cast<size_t>(CONTROLS::SCROLL_DOWN)] = ControlState::CONTROLSTATE_UP;
	}
	else {
		this->controlState[static_cast<size_t>(CONTROLS::SCROLL_UP)] = ControlState::CONTROLSTATE_UP;
		this->controlState[static_cast<size_t>(CONTROLS::SCROLL_DOWN)] = ControlState::CONTROLSTATE_UP;
	}
}
