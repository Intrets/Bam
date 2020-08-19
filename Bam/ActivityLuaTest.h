#pragma once

#include <unordered_set>
#include <string>
#include <sol/sol.hpp>

class GameState;
class Saver;
class Loader;
struct RenderInfo;
typedef int32_t Handle;

class ActivityLuaTest
{
public:
	sol::state state;
	GameState* gameStateRef;

	std::function<void(std::string line)> printFunction = [](std::string)
	{};

	std::unordered_set<std::string> ignore;

	bool applyActivity(Handle h, int32_t type);

	void runScript(GameState& gameState, Handle h);
	void save(Saver& saver);
	void load(Loader& loader);

	void print(std::string string);

	ActivityLuaTest();
	~ActivityLuaTest() = default;

protected:
	virtual void appendRenderInfoInternal(GameState& gameState, RenderInfo& renderInfo);
};

