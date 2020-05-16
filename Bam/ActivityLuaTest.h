#pragma once

#include "ActivitySelector.h"

#include "sol/sol.hpp"

class GameState;

class ActivityLuaTest : public ActivitySelector 
{
public:
	sol::state state;
	GameState* gameStateRef;

	bool applyActivity(Handle h, int32_t type);

	void runScript(GameState& gameState);

	ActivityLuaTest();
	~ActivityLuaTest() = default;
};

