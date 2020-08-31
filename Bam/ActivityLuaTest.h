#pragma once

#include <unordered_set>
#include <string>
#include <sol/sol.hpp>

class GameState;
class Saver;
class Loader;
struct RenderInfo;
typedef int32_t Handle;
class Activity;

class ActivityLuaTest
{
private:
	// The target Activity and its group to act on.
	Activity* target;

	std::function<void(std::string line)> printFunction = [](std::string)
	{};

	friend class LuaActivity;

public:
	sol::state state;
	GameState* gameStateRef;

	std::string script;

	bool applyMove(Handle h, int32_t type);
	bool applyActivity(Handle h, int32_t type);

	void runScript(GameState& gameState);
	void save(Saver& saver);
	void load(Loader& loader);

	ActivityLuaTest();
	ActivityLuaTest(Activity& ptr);
	~ActivityLuaTest() = default;

protected:
	virtual void appendRenderInfoInternal(GameState& gameState, RenderInfo& renderInfo);
};

