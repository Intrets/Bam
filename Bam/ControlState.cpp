#include "common.h"
#include "ControlState.h"

#include <array>
#include <iostream>

ControlState::ControlState() {
	this->controlState.fill(0);

	this->keyToControl[GLFW_KEY_A] = CONTROL::KEY::LEFT;
	this->keyToControl[GLFW_KEY_D] = CONTROL::KEY::RIGHT;
	this->keyToControl[GLFW_KEY_W] = CONTROL::KEY::UP;
	this->keyToControl[GLFW_KEY_S] = CONTROL::KEY::DOWN;
	this->keyToControl[GLFW_KEY_Q] = CONTROL::KEY::ACTION_PICK;
	this->keyToControl[GLFW_KEY_F] = CONTROL::KEY::ACTION_DELETE;
	this->keyToControl[GLFW_KEY_E] = CONTROL::KEY::ACTION_USE;
	this->keyToControl[GLFW_KEY_R] = CONTROL::KEY::ROTATER;
	this->keyToControl[GLFW_KEY_Z] = CONTROL::KEY::ACTION_ACTIVATE;
	this->keyToControl[GLFW_KEY_LAST + GLFW_MOUSE_BUTTON_1] = CONTROL::KEY::ACTION0;
	this->keyToControl[GLFW_KEY_LAST + GLFW_MOUSE_BUTTON_2] = CONTROL::KEY::ACTION1;
	this->keyToControl[GLFW_KEY_LAST + GLFW_MOUSE_BUTTON_3] = CONTROL::KEY::ACTION2;
	this->keyToControl[GLFW_KEY_A] = CONTROL::KEY::LEFT;
	this->keyToControl[GLFW_KEY_D] = CONTROL::KEY::RIGHT;
	this->keyToControl[GLFW_KEY_S] = CONTROL::KEY::DOWN;
	this->keyToControl[GLFW_KEY_W] = CONTROL::KEY::UP;
	this->keyToControl[GLFW_KEY_F7] = CONTROL::KEY::TEST_SAVE;
	this->keyToControl[GLFW_KEY_F8] = CONTROL::KEY::TEST_LOAD;
	this->keyToControl[GLFW_KEY_F9] = CONTROL::KEY::TOGGLE_DEBUG;
	this->keyToControl[GLFW_KEY_ESCAPE] = CONTROL::KEY::CANCEL;

	this->keyToControl[GLFW_KEY_UP] = CONTROL::KEY::TEXT_UP;
	this->keyToControl[GLFW_KEY_DOWN] = CONTROL::KEY::TEXT_DOWN;
	this->keyToControl[GLFW_KEY_LEFT] = CONTROL::KEY::TEXT_LEFT;
	this->keyToControl[GLFW_KEY_RIGHT] = CONTROL::KEY::TEXT_RIGHT;

	this->keyToControl[GLFW_KEY_1] = CONTROL::KEY::TOOL_1;
	this->keyToControl[GLFW_KEY_2] = CONTROL::KEY::TOOL_2;
	this->keyToControl[GLFW_KEY_3] = CONTROL::KEY::TOOL_3;
	this->keyToControl[GLFW_KEY_4] = CONTROL::KEY::TOOL_4;
	this->keyToControl[GLFW_KEY_5] = CONTROL::KEY::TOOL_5;
	this->keyToControl[GLFW_KEY_6] = CONTROL::KEY::TOOL_6;
	this->keyToControl[GLFW_KEY_7] = CONTROL::KEY::TOOL_7;
	this->keyToControl[GLFW_KEY_8] = CONTROL::KEY::TOOL_8;
	this->keyToControl[GLFW_KEY_9] = CONTROL::KEY::TOOL_9;
	this->keyToControl[GLFW_KEY_0] = CONTROL::KEY::TOOL_0;

	this->keyToControl[GLFW_KEY_BACKSPACE] = CONTROL::KEY::BACKSPACE;
	this->keyToControl[GLFW_KEY_DELETE] = CONTROL::KEY::DELETE;
	this->keyToControl[GLFW_KEY_TAB] = CONTROL::KEY::TAB;
}

void ControlState::cycleStates() {
	for (auto& state : this->controlState) {
		state &= ~CONTROL::STATE::PRESSED;
		state &= ~CONTROL::STATE::RELEASED;
		state &= ~CONTROL::STATE::REPEAT;
	}
	this->charBuffer.clear();
	this->scrollDistance = 0;
	this->blockUserInput = false;
	this->controlState[static_cast<int32_t>(CONTROL::KEY::MOUSE_POS_CHANGED)] = CONTROL::STATE::PRESSED;
	this->controlState[static_cast<int32_t>(CONTROL::KEY::EVERY_TICK)] = CONTROL::STATE::PRESSED;

	this->consumed.fill(false);
	this->consumedBuffer.fill(false);

	this->blockWorldBinds = false;
}

std::string ControlState::getCharBuffer() {
	return this->charBuffer;
}

void ControlState::setBlockWorldBinds(bool b) {
	this->blockWorldBinds = b;
}

bool ControlState::worldBindsBlocked() {
	return this->blockWorldBinds;
}

void ControlState::consumeControl(CONTROL::KEY control) {
	this->consumed[static_cast<size_t>(control)] = true;
}

void ControlState::consumeBufferControl(CONTROL::KEY control) {
	this->consumedBuffer[static_cast<size_t>(control)] = true;
}

void ControlState::writeConsumedBuffer() {
	for (int32_t i = 0; i < static_cast<int32_t>(CONTROL::KEY::CONTROLS_MAX); i++) {
		this->consumed[i] = this->consumed[i] | this->consumedBuffer[i];
	}
	this->consumedBuffer.fill(false);
}

bool ControlState::activated(BindControl bindControl) {
	if (this->consumed[static_cast<size_t>(bindControl.control)]) {
		return false;
	}

	if (this->blockUserInput) {
		switch (bindControl.control) {
			case CONTROL::KEY::CHAR_BUFFER_CHANGED:
			case CONTROL::KEY::EVERY_TICK:
			case CONTROL::KEY::MOUSE_POS_CHANGED:
			case CONTROL::KEY::ACTION0:
				break;
			default:
				return false;
				break;
		}
	}

	if ((this->modifiers & bindControl.modifiers) != bindControl.modifiers) {
		return false;
	}

	return this->controlState[static_cast<size_t>(bindControl.control)] & bindControl.state;
}

void ControlState::key_callback(GLFWwindow* w, int32_t key, int32_t scancode, int32_t action, int32_t mods) {
	// unknown key?
	if (key == -1) {
		return;
	}

	if (key == GLFW_KEY_ENTER && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		this->charBuffer.push_back('\n');
		this->controlState[static_cast<size_t>(CONTROL::KEY::CHAR_BUFFER_CHANGED)] = CONTROL::STATE::PRESSED;
	}

	switch (key) {
		case GLFW_KEY_LEFT_SHIFT:
		case GLFW_KEY_RIGHT_SHIFT:
			if (action == GLFW_PRESS) {
				this->modifiers &= ~CONTROL::MODIFIER::NONE;
				this->modifiers |= CONTROL::MODIFIER::SHIFT;
			}
			else if (action == GLFW_RELEASE) {
				this->modifiers &= ~CONTROL::MODIFIER::SHIFT;
			}
			break;
		case GLFW_KEY_LEFT_CONTROL:
		case GLFW_KEY_RIGHT_CONTROL:
			if (action == GLFW_PRESS) {
				this->modifiers &= ~CONTROL::MODIFIER::NONE;
				this->modifiers |= CONTROL::MODIFIER::CONTROL;
			}
			else if (action == GLFW_RELEASE) {
				this->modifiers &= ~CONTROL::MODIFIER::CONTROL;
			}
			break;
		case GLFW_KEY_LEFT_ALT:
		case GLFW_KEY_RIGHT_ALT:
			if (action == GLFW_PRESS) {
				this->modifiers &= ~CONTROL::MODIFIER::NONE;
				this->modifiers |= CONTROL::MODIFIER::ALT;
			}
			else if (action == GLFW_RELEASE) {
				this->modifiers &= ~CONTROL::MODIFIER::ALT;
			}
			break;
		default:
			break;
	}

	if (this->modifiers == 0) {
		this->modifiers = CONTROL::MODIFIER::NONE;
	}

	switch (action) {
		case GLFW_REPEAT:
			this->controlState[static_cast<size_t>(keyToControl[key])] |= CONTROL::STATE::REPEAT;
			break;
		case GLFW_PRESS:
			this->controlState[static_cast<size_t>(keyToControl[key])] |= CONTROL::STATE::DOWN;
			this->controlState[static_cast<size_t>(keyToControl[key])] |= CONTROL::STATE::PRESSED;
			break;
		case GLFW_RELEASE:
			this->controlState[static_cast<size_t>(keyToControl[key])] &= ~CONTROL::STATE::DOWN;
			this->controlState[static_cast<size_t>(keyToControl[key])] |= CONTROL::STATE::RELEASED;
			break;
		default:
			break;
	}
}

void ControlState::char_callback(GLFWwindow* window, unsigned int character) {
	if (character > 127) {
		return;
	}
	this->controlState[static_cast<size_t>(CONTROL::KEY::CHAR_BUFFER_CHANGED)] = CONTROL::STATE::PRESSED;
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
		this->controlState[static_cast<size_t>(CONTROL::KEY::SCROLL_DOWN)] = CONTROL::STATE::PRESSED;
		this->controlState[static_cast<size_t>(CONTROL::KEY::SCROLL_UP)] = CONTROL::STATE::UP;
	}
	else if (scrollDistance > 0) {
		this->controlState[static_cast<size_t>(CONTROL::KEY::SCROLL_UP)] = CONTROL::STATE::PRESSED;
		this->controlState[static_cast<size_t>(CONTROL::KEY::SCROLL_DOWN)] = CONTROL::STATE::UP;
	}
	else {
		this->controlState[static_cast<size_t>(CONTROL::KEY::SCROLL_UP)] = CONTROL::STATE::UP;
		this->controlState[static_cast<size_t>(CONTROL::KEY::SCROLL_DOWN)] = CONTROL::STATE::UP;
	}
}

BindControl::BindControl(CONTROL::KEY c) :
	BindControl(c, CONTROL::STATE::PRESSED) {
}

BindControl::BindControl(CONTROL::KEY c, int32_t s) :
	BindControl(c, s, CONTROL::MODIFIER::ANY) {
}

BindControl::BindControl(CONTROL::KEY c, int32_t s, int32_t m) :
	control(c),
	state(s),
	modifiers(m) {
}
