#include "common.h"
#include "BindHandler.h"

#include <functional>
//#include "ReferenceManager.h"
//#include "Activity.h"
//#include "Platform.h"
#include "ControlState.h"
#include "GameState.h"

void BindHandler::runBinds(ControlState& controlState, GameState& gameState) {
	for (int i = 0; i < CONTROLS::CONTROLS_MAX; i++) {
		CONTROLS control = static_cast<CONTROLS>(i);
		CONTROLSTATE state = controlState.controlState[control];

		for (auto& bind : binds[control][state]) {
			bind(gameState);
		}

		//CONTROLS control = static_cast<CONTROLS>(i);
		//if (controlState.justChanged[control]) {
		//	int index = bindToIndex(control, controlState.controlState[control]);
		//	for (auto& bind : binds[index]) {
		//		bind(gameState);
		//	}
		//}
	}
}

void BindHandler::addBind(CONTROLS control, CONTROLSTATE state, std::function<void(GameState&)> f) {
	binds[control][state].push_back(f);
}

BindHandler::BindHandler() {
}

BindHandler::~BindHandler() {
}
