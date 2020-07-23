#pragma once

#include "ActivitySelector.h"

#ifndef SOL_DEFINE
#define SOL_DEFINE
#include "sol/sol.hpp"
#endif 

class GameState;
class Saver;
class Loader;

class ActivityLuaTest 
{
public:
	std::string text;
	sol::state state;
	GameState* gameStateRef;

	std::unordered_set<std::string> ignore;

	bool applyActivity(Handle h, int32_t type);

	void runScript(GameState& gameState);
	void save(Saver& saver);
	void load(Loader& loader);

	ActivityLuaTest();
	~ActivityLuaTest() = default;

protected:
	virtual void appendRenderInfoInternal(GameState& gameState, RenderInfo& renderInfo);
};

